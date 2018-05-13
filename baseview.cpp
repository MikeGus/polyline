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
    }
}

void baseview::updateRouteStats(size_t selectedRow) {
    routeTableWidget->setItem(selectedRow, 1, new QTableWidgetItem(QString::number(mediator->getRoute(selectedRow).length(), 'f', 1)));
    routeTableWidget->setItem(selectedRow, 2, new QTableWidgetItem(QDateTime::currentDateTime().toString()));

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
}

void baseview::removeRoute(size_t position) {
    if (position == getSelectedRow(routeTableWidget)) {
        pointTableWidget->clearContents();
        pointTableWidget->setRowCount(0);
        polylineText->clear();
    }
    routeTableWidget->removeRow(position);
}

void baseview::addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position) {

    updateRouteStats(routeIndex);

    if (routeIndex != getSelectedRow(routeTableWidget)) {
        updateCurrentRoute(routeIndex);
    }

    pointTableWidget->insertRow(position);
    pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(waypoint.getLatitude(), 'f', 5)));
    pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(waypoint.getLongitude(), 'f', 5)));
}

void baseview::removeWaypoint(size_t routeIndex, size_t position) {
    updateRouteStats(routeIndex);

    if (routeIndex != getSelectedRow(routeTableWidget)) {
        updateCurrentRoute(routeIndex);
    } else {
        pointTableWidget->removeRow(position);
    }
}

void baseview::editWaypoint(size_t routeIndex, coordinates& editWaypoint,
                              size_t position) {
    updateCurrentRoute(routeIndex);
    updateRouteStats(routeIndex);

    pointTableWidget->setItem(position, 0,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLatitude(), 'f', 5)));
    pointTableWidget->setItem(position, 1,
                                  new QTableWidgetItem(QString::number(editWaypoint.getLongitude(), 'f', 5)));
}

size_t baseview::getSelectedRow(const QTableWidget* table) const {
    if (table->selectedItems().empty()) {
        return -1;
    }

    return table->selectedItems().first()->row();
}
