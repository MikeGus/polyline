#include "presenter.h"
#include <QDateTime>
#include <QFileInfo>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QVector>

#include "polyline.h"
#include "command/addroutecommand.h"
#include "command/addwaypointcommand.h"
#include "command/deleteroutecommand.h"
#include "command/deletewaypointcommand.h"
#include "command/editwaypointcommand.h"
#include "baseview.h"

presenter::presenter(QUndoStack* undoStack, routemanager* routeManager, baseview* view, QObject *parent) :
    QObject(parent), undoStack(undoStack), routeManager(routeManager), view(view) {
}

void presenter::addSampleRoute(QString& name) {
    if (name.isEmpty()) {
        emit displayError("Путь должен иметь название");
        return;
    }

    QVector<route> newRoutes;
    newRoutes.push_back(route(name, QDateTime::currentDateTime().toString()));
    undoStack->push(new addroutecommand(routeManager, newRoutes));
}

void presenter::addRouteFromPolyline(QString& name, QString& poly) {
    if (name.isEmpty()) {
        emit displayError("Путь должен иметь название");
        return;
    }
    try {
        polyline routePolyline(poly.toStdString());

        QVector<route> newRoutes;
        newRoutes.push_back(route(name, QDateTime::currentDateTime().toString(), routePolyline));
        undoStack->push(new addroutecommand(routeManager, newRoutes));
    } catch(std::logic_error& er) {
        emit displayError("Некорректный полилайн");
    }
}

void presenter::addRouteFromFiles(QStringList& filenames) {
    try {
        QVector<route> newRoutes;
        for (auto& filename : filenames) {
            route newRoute;
            newRoute.readFromFile(filename);

            newRoutes.push_back(newRoute);
        }
        undoStack->push(new addroutecommand(routeManager, newRoutes));
    } catch(std::invalid_argument& er) {
        emit displayError(er.what());
    }
}

void presenter::removeRoute(size_t selectedRoute) {
    if (selectedRoute >= routeManager->size()) {
        emit displayError("Недопустимый путь");
        return;
    }
    QVector<route> delRoutes;
    delRoutes.push_back(routeManager->at(selectedRoute));
    QVector<size_t> selectedRoutes;
    selectedRoutes.push_back(selectedRoute);

    undoStack->push(new deleteroutecommand(routeManager, delRoutes, selectedRoutes));
}

void presenter::addWaypoint(size_t selectedRoute, coordinates& newPoint) {
    if (selectedRoute >= routeManager->size()) {
        emit displayError("Недопустимый путь");
        return;
    }
    if (fabs(newPoint.getLatitude()) > 90 || fabs(newPoint.getLongitude() > 180)) {
        emit displayError("Недопустимые координаты");
        return;
    }

    QVector<coordinates> newPoints;
    newPoints.push_back(newPoint);

    undoStack->push(new addwaypointcommand(routeManager, selectedRoute, newPoints));
}

void presenter::editWaypoint(size_t selectedRoute, size_t selectedPoint, coordinates& newPoint) {
    if (selectedRoute >= routeManager->size()) {
        emit displayError("Недопустимый путь");
        return;
    }
    if (selectedPoint >= routeManager->at(selectedRoute).getNumberOfPoints()) {
        emit displayError("Недопустимая позиция для изменения точки");
        return;
    }
    if (fabs(newPoint.getLatitude()) > 90 || fabs(newPoint.getLongitude() > 180)) {
        emit displayError("Недопустимые координаты");
        return;
    }

    QVector<coordinates> newPoints;
    newPoints.push_back(newPoint);

    QVector<coordinates> oldPoints;
    oldPoints.push_back(routeManager->at(selectedRoute)[selectedPoint]);

    QVector<size_t> selectedPoints;
    selectedPoints.push_back(selectedPoint);

    undoStack->push(new editwaypointcommand(routeManager, selectedRoute, newPoints,
                                            oldPoints,
                                            selectedPoints));
}

void presenter::removeWaypoint(size_t selectedRoute, size_t selectedPoint) {
    if (selectedRoute >= routeManager->size()) {
        emit displayError("Недопустимый путь");
        return;
    }
    if (selectedPoint >= routeManager->at(selectedRoute).getNumberOfPoints()) {
        emit displayError("Недопустимая позиция для изменения точки");
        return;
    }
    QVector<size_t> selectedPoints;
    selectedPoints.push_back(selectedPoint);

    QVector<coordinates> oldPoints;
    oldPoints.push_back(routeManager->at(selectedRoute)[selectedPoint]);

    undoStack->push(new deletewaypointcommand(routeManager, selectedRoute, oldPoints,
                                              selectedPoints));
}

void presenter::saveState() {
    QString filename(backupPath);
    QFile backup(filename);

    backup.open(QIODevice::WriteOnly);

    QXmlStreamWriter writer(&backup);
    writer.setAutoFormatting(true);

    writer.writeStartDocument();

    writer.writeStartElement("routes");
    for (size_t i = 0; i < routeManager->size(); ++i) {
        writer.writeStartElement("route");

        writer.writeStartElement("name");
        writer.writeCharacters(routeManager->at(i).getName());
        writer.writeEndElement();

        writer.writeStartElement("date");
        writer.writeCharacters(routeManager->at(i).getDate());
        writer.writeEndElement();

        writer.writeStartElement("waypoints");
        for (size_t j = 0; j < routeManager->at(i).getNumberOfPoints(); ++j) {
            writer.writeStartElement("waypoint");

            QXmlStreamAttributes attr;
            attr.append("latitude", QString::number(routeManager->at(i)[j].getLatitude()));
            attr.append("longitude", QString::number(routeManager->at(i)[j].getLongitude()));
            attr.append("height", QString::number(routeManager->at(i)[j].getHeight()));
            writer.writeAttributes(attr);

            writer.writeEndElement();
        }
        writer.writeEndElement();

        writer.writeEndElement();
    }
    writer.writeEndElement();

    writer.writeEndDocument();
}


void presenter::loadState() {
    QString filename(backupPath);
    QFile backup(filename);

    QVector<route> newRoutes;

    if (!backup.open(QIODevice::ReadOnly | QFile::Text)) {
        return;
    }

    QXmlStreamReader reader(&backup);

    if (reader.readNextStartElement()) {
        if (reader.name() == "routes") {
            while (reader.readNextStartElement()) {
                if (reader.name() == "route") {
                    route newRoute;
                    while (reader.readNextStartElement()) {
                        if (reader.name() == "name") {
                            newRoute.setName(reader.readElementText());
                        } else if (reader.name() == "date") {
                            newRoute.setDate(reader.readElementText());
                        } else if (reader.name() == "waypoints") {
                            while (reader.readNextStartElement()) {
                                if (reader.name() == "waypoint") {
                                    QXmlStreamAttributes attr = reader.attributes();
                                    coordinates waypoint;

                                    waypoint.setLatitude(attr.value("latitude").toDouble());
                                    waypoint.setLongitude(attr.value("longitude").toDouble());
                                    waypoint.setHeight(attr.value("height").toDouble());

                                    newRoute.addLast(waypoint);
                                    reader.skipCurrentElement();
                                } else {
                                    reader.skipCurrentElement();
                                }
                            }
                        } else {
                            reader.skipCurrentElement();
                        }
                    }
                    newRoutes.push_back(newRoute);
                } else {
                    reader.skipCurrentElement();
                }
            }
        } else {
            reader.skipCurrentElement();
        }
    }
    undoStack->push(new addroutecommand(routeManager, newRoutes));
}

void presenter::addRouteToView(route& newRoute, size_t position) {
    view->addRoute(newRoute, position);
}

void presenter::removeRouteFromView(size_t position) {
    view->removeRoute(position);
}

void presenter::addWaypointToView(size_t routePosition, coordinates& waypoint,
                       size_t waypointPosition) {
    view->addWaypoint(routePosition, waypoint, waypointPosition);
}

void presenter::removeWaypointFromView(size_t routePosition, size_t waypointPosition) {
    view->removeWaypoint(routePosition, waypointPosition);
}

void presenter::editWaypointInView(size_t routePosition, coordinates& waypoint,
                                   size_t waypointPosition) {
    view->editWaypoint(routePosition, waypoint, waypointPosition);
}

route& presenter::getRoute(size_t routePosition) {
    return routeManager->at(routePosition);
}
