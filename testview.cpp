#include "testview.h"
#include <iostream>

testview::testview(QWidget* parent): baseview(parent) {

    routeTableWidget = new QTableWidget();
    pointTableWidget = new QTableWidget();
    plot = new QCustomPlot();
    polylineText = new QTextBrowser();
}

testview::~testview() {

    delete polylineText;
    delete plot;
    delete routeTableWidget;
    delete pointTableWidget;
}

void testview::displayError(const char *msg) {
    std::cerr << "Error: " << msg << std::endl;
}

void testview::clear() {
    routeTableWidget->setRowCount(0);
    pointTableWidget->setRowCount(0);
}

int testview::pointNumber() {
    return pointTableWidget->rowCount();
}

int testview::routeNumber() {
    return routeTableWidget->rowCount();
}

void testview::setMediator(presenter* mediator) {
    this->mediator = mediator;
}

route testview::getRoute(int position) {

    QTableWidgetItem* name = routeTableWidget->item(position, 0);
    QTableWidgetItem* date = routeTableWidget->item(position, 1);

    route result(name->text(), date->text());

    return result;
}

coordinates testview::getCoordinates(int position) {

    QTableWidgetItem* latitude =  pointTableWidget->item(position, 0);
    QTableWidgetItem* longitude = pointTableWidget->item(position, 1);
    QTableWidgetItem* height = pointTableWidget->item(position, 2);

    coordinates result(latitude->text().toDouble(), longitude->text().toDouble(),
                       height->text().toDouble());

    return result;
}
