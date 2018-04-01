#include "presenter.h"
#include <QDateTime>
#include <QFileInfo>

#include "polyline.h"
#include "command/addroutecommand.h"
#include "command/addwaypointcommand.h"
#include "command/deleteroutecommand.h"
#include "command/deletewaypointcommand.h"
#include "command/editwaypointcommand.h"

presenter::presenter(QUndoStack* undoStack, routemanager* routeManager, QObject *parent) :
    QObject(parent), undoStack(undoStack), routeManager(routeManager) {
}

void presenter::addSampleRoute(QString& name) {
    if (name.isEmpty()) {
        emit displayError("Путь должен иметь название");
        return;
    }

    route newRoute(name, QDateTime::currentDateTime().toString());
    undoStack->push(new addroutecommand(routeManager, newRoute));
}

void presenter::addRouteFromPolyline(QString& name, QString& poly) {
    if (name.isEmpty()) {
        emit displayError("Путь должен иметь название");
        return;
    }
    try {
        polyline routePolyline(poly.toStdString());
        route newRoute(name, QDateTime::currentDateTime().toString(), routePolyline);
        undoStack->push(new addroutecommand(routeManager, newRoute));
    } catch(std::logic_error& er) {
        emit displayError(er.what());
    }
}

void presenter::addRouteFromFile(QString& filename) {
    try {
        QFileInfo fileInfo(filename);
        route newRoute(filename, fileInfo.metadataChangeTime().toString());
        newRoute.readFromFile(filename);

        undoStack->push(new addroutecommand(routeManager, newRoute));
    } catch(std::logic_error& er) {
        emit displayError(er.what());
    }
}

void presenter::removeRoute(size_t selectedRoute) {
    if (selectedRoute >= routeManager->size()) {
        emit displayError("Недопустимый путь");
        return;
    }
    undoStack->push(new deleteroutecommand(routeManager, routeManager->at(selectedRoute), selectedRoute));
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

    undoStack->push(new addwaypointcommand(routeManager, selectedRoute, newPoint));
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

    undoStack->push(new editwaypointcommand(routeManager, selectedRoute, newPoint,
                                            routeManager->at(selectedRoute)[selectedPoint],
                                            selectedPoint));
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
    undoStack->push(new deletewaypointcommand(routeManager, selectedRoute, routeManager->at(selectedRoute)[selectedPoint],
                                              selectedPoint));
}