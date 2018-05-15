#include "baseview.h"
#include <QDateTime>

baseview::baseview(QWidget *parent): QMainWindow(parent) {

}

void baseview::updateCurrentRoute(size_t selectedRow) {

    routeTableWidget->clearSelection();
    routeTableWidget->selectRow(selectedRow);
    polylineText->setText(QString::fromStdString(mediator->getRoute(selectedRow).generatePolyline().get()));

    pointTableWidget->clearContents();
    size_t numberOfPoints = mediator->getRoute(selectedRow).getNumberOfPoints();

    pointTableWidget->setRowCount(numberOfPoints);
    for (size_t i = 0; i < numberOfPoints; ++i) {
        pointTableWidget->setItem(i, 0,
                                      new QTableWidgetItem(QString::number(mediator->getRoute(selectedRow)[i].getLatitude())));
        pointTableWidget->setItem(i, 1,
                                      new QTableWidgetItem(QString::number(mediator->getRoute(selectedRow)[i].getLongitude())));
        pointTableWidget->setItem(i, 2,
                                      new QTableWidgetItem(QString::number(mediator->getRoute(selectedRow)[i].getHeight())));
    }
}

void baseview::updateRouteStats(size_t selectedRow) {
    routeTableWidget->setItem(selectedRow, 1, new QTableWidgetItem(QString::number(mediator->getRoute(selectedRow).length(), 'f', 1)));
    mediator->getRoute(selectedRow).setDate(QDateTime::currentDateTime().toString());
    routeTableWidget->setItem(selectedRow, 2, new QTableWidgetItem(mediator->getRoute(selectedRow).getDate()));

    if (selectedRow == getSelectedRow(routeTableWidget)) {
        polylineText->setText(QString::fromStdString(mediator->getRoute(selectedRow).generatePolyline().get()));
    }
}

void baseview::addRoute(route& newRoute, size_t position) {
    QTableWidgetItem* routeName = new QTableWidgetItem(newRoute.getName());
    QTableWidgetItem* routeLength = new QTableWidgetItem(QString::number(newRoute.length(), 'f', 1));
    QTableWidgetItem* routeDate = new QTableWidgetItem(newRoute.getDate());

    routeTableWidget->insertRow(position);
    routeTableWidget->setItem(position, 0, routeName);
    routeTableWidget->setItem(position, 1, routeLength);
    routeTableWidget->setItem(position, 2, routeDate);

    updateCurrentRoute(position);
    updatePlot(position);
}

void baseview::removeRoute(size_t position) {
    if (position == getSelectedRow(routeTableWidget)) {
        pointTableWidget->clearContents();
        pointTableWidget->setRowCount(0);
        polylineText->clear();

        plot->clearGraphs();
    }
    routeTableWidget->removeRow(position);
}

void baseview::addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position) {

    updateRouteStats(routeIndex);

    if (routeIndex != getSelectedRow(routeTableWidget)) {
        updateCurrentRoute(routeIndex);
    } else {
        pointTableWidget->insertRow(position);
        pointTableWidget->setItem(position, 0,
                                      new QTableWidgetItem(QString::number(waypoint.getLatitude(), 'f', 5)));
        pointTableWidget->setItem(position, 1,
                                      new QTableWidgetItem(QString::number(waypoint.getLongitude(), 'f', 5)));
        pointTableWidget->setItem(position, 2,
                                      new QTableWidgetItem(QString::number(waypoint.getHeight(), 'f', 5)));
    }

    updatePlot(routeIndex);
}

void baseview::removeWaypoint(size_t routeIndex, size_t position) {
    updateRouteStats(routeIndex);

    if (routeIndex != getSelectedRow(routeTableWidget)) {
        updateCurrentRoute(routeIndex);
    } else {
        pointTableWidget->removeRow(position);
    }

    updatePlot(routeIndex);
}

void baseview::editWaypoint(size_t routeIndex, coordinates& editWaypoint,
                              size_t position) {
    updateCurrentRoute(routeIndex);
    updateRouteStats(routeIndex);
    updatePlot(routeIndex);

    pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLatitude(), 'f', 5)));
    pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLongitude(), 'f', 5)));
    pointTableWidget->setItem(position, 2,
                                  new QTableWidgetItem(QString::number(editWaypoint.getHeight(), 'f', 5)));
}

size_t baseview::getSelectedRow(const QTableWidget* table) const {
    if (table->selectedItems().empty()) {
        return -1;
    }

    return table->selectedItems().first()->row();
}

void baseview::updatePlot(size_t selectedRow) {
    plot->clearGraphs();
    route currentRoute(mediator->getRoute(selectedRow));

    if (currentRoute.getNumberOfPoints() == 0) {
        return;
    }

    plot->addGraph();

    QVector<double> x = {0};
    QVector<double> y = {currentRoute[0].getHeight()};

    double distance = 0;
    for (int i = 1; i < pointTableWidget->rowCount(); ++i) {
        distance += currentRoute[i].distance(currentRoute[i - 1]);
        x.append(distance / 1000);
        y.append(currentRoute[i].getHeight());
    }

    double maxY = *std::max_element(y.begin(), y.end());
    double minY = *std::min_element(y.begin(), y.end());


    plot->yAxis->setRange(minY - 10, maxY + 10);
    plot->xAxis->setRange(x.first(), x.last());

    plot->graph(0)->setData(x, y);
    plot->replot();
}
