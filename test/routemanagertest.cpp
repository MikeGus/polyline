#include "routemanagertest.h"
#include "../presenter.h"

void routemanagertest::routeDelete_1() {

    routemanager manager;

    QString name1("ASKjdksaj"), name2("KSKnds"), name3("OSADIAJ");
    route route1(name1, QString()), route2(name2, QString()), route3(name3, QString());

    QCOMPARE(manager.size(), 0);

    manager.addRoute(route1, 0);
    manager.addRoute(route2, 1);
    manager.addRoute(route3, 2);

    QCOMPARE(manager.size(), 3);

    manager.removeRoute(0);

    QCOMPARE(manager.size(), 2);
    QCOMPARE(manager.at(0).getName(), name2);

    manager.removeRoute(0);

    QCOMPARE(manager.at(0).getName(), name3);
    QCOMPARE(manager.size(), 1);
}

void routemanagertest::routeDelete_2() {
    routemanager manager;
    manager.removeRoute(0);
}

void routemanagertest::routeDelete_3() {
    routemanager manager;
    QString name1("ASKjdksaj");
    route route1(name1, QString());

    manager.addRoute(route1, 0);
    manager.removeRoute(0);
    manager.removeRoute(0);
    manager.addRoute(route1, 0);
    manager.addRoute(route1, 0);
    manager.addRoute(route1, 0);

    QCOMPARE(manager.size(), 3);
}
