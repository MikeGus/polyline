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
    updatePlot(position);
}

void baseview::removeRoute(size_t position) {
    if (position == getSelectedRow(routeTableWidget)) {
        pointTableWidget->clearContents();
        pointTableWidget->setRowCount(0);
        polylineText->clear();

        plot->clearGraphs();
        plot->close();
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
}

size_t baseview::getSelectedRow(const QTableWidget* table) const {
    if (table->selectedItems().empty()) {
        return -1;
    }

    return table->selectedItems().first()->row();
}

void baseview::updatePlot(size_t selectedRow) {
    plot->close();
    plot->addGraph();

    QVector<double> x;
    QVector<double> y;
    for (int i = 0; i < 1000; ++i) {
        x.append(i);
        y.append(i * i);
    }

    double maxY = *std::max_element(y.begin(), y.end());
    double minY = *std::min_element(y.begin(), y.end());

    double maxX = *std::max_element(x.begin(), x.end());
    double minX = *std::min_element(x.begin(), x.end());

    plot->yAxis->setRange(minY - 10, maxY + 10);
    plot->xAxis->setRange(minX, maxX);

    plot->graph(0)->setData(x, y);
    plot->replot();

    plot->show();
}
