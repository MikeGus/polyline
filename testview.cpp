#include "testview.h"
#include <iostream>

testview::testview(QWidget* parent): baseview(parent) {

    routeTableWidget = new QTableWidget(this);
    pointTableWidget = new QTableWidget(this);
}

testview::~testview() {

    delete routeTableWidget;
    delete pointTableWidget;
}

void testview::displayError(const char *msg) {
    std::cerr << "Error: " << msg << std::endl;
}
