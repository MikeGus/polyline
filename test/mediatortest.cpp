#include "mediatortest.h"
#include "../testview.h"
#include "../presenter.h"
#include "../routemanager.h"
#include <QUndoStack>
#include <QUndoCommand>
#include <QFile>

mediatortest::mediatortest() {
    stack = new QUndoStack;
    view = new testview;
    manager = new routemanager;
    mediator = new presenter(stack, manager, view);
    view->setMediator(mediator);

    setupSignals();
}

mediatortest::~mediatortest() {
    delete mediator;
    delete manager;
    delete view;
    delete stack;
}

void mediatortest::routeListCorrect() {
    clear();
    QFile::remove(backupPath);

    QFile::copy("savedstate1.dat", backupPath);
    mediator->loadState();
    QCOMPARE(view->routeNumber(), 2);
    QCOMPARE(view->pointNumber(), 3);
    QCOMPARE(view->getRoute(0), route("route1",
                                      "вт мая 15 21:19:32 2018"));

    QCOMPARE(view->getRoute(1), route("route2",
                                      "вт мая 15 21:19:54 2018"));
    QCOMPARE(view->getCoordinates(0), coordinates(0, 2, 1));
    QCOMPARE(view->getCoordinates(1), coordinates(5, 2, 1));
    QCOMPARE(view->getCoordinates(2), coordinates(5, 3, 1));
}

void mediatortest::routeListIncorrect() {
    clear();
    QFile::remove(backupPath);
    QFile::copy("savedstate2.dat", backupPath);
    mediator->loadState();
    QCOMPARE(view->routeNumber(), 0);
    QCOMPARE(view->pointNumber(), 0);
}

void mediatortest::createRouteCorrect() {
    clear();
    QString name("1");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);
    QCOMPARE(view->routeNumber(), 1);
    QCOMPARE(view->getRoute(0).getName(), name);
}

void mediatortest::createRouteIncorrect() {
    clear();
    QString name("saddas");
    QString poly("aflfasjlsakj");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addRouteFromPolyline(name, poly);
    QCOMPARE(view->routeNumber(), 0);
}

void mediatortest::deleteRouteCorrect() {
    clear();
    QString name("1");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);
    mediator->addSampleRoute(name);
    mediator->addSampleRoute(name);
    mediator->addSampleRoute(name);
    QCOMPARE(view->routeNumber(), 4);
    mediator->removeRoute(0);
    QCOMPARE(view->routeNumber(), 3);
}

void mediatortest::deleteRouteInCorrect() {
    clear();
    QCOMPARE(view->routeNumber(), 0);
    mediator->removeRoute(0);
    QCOMPARE(view->routeNumber(), 0);
}

void mediatortest::createPointCorrect() {
    clear();
    QString name("1");
    coordinates correct(0, 0, 14);
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, correct);
    QCOMPARE(view->pointNumber(), 1);
    QCOMPARE(view->getCoordinates(0), correct);
}

void mediatortest::createPointInCorrect() {
    clear();
    QString name("1");
    coordinates incorrect(0, 1800, 14);
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);
    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, incorrect);
    QCOMPARE(view->pointNumber(), 0);
}

void mediatortest::deletePointCorrect() {
    clear();
    QString name("1");
    coordinates correct(0, 0, 14);
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, correct);
    correct.setHeight(25);
    mediator->addWaypoint(0, correct);
    QCOMPARE(view->pointNumber(), 2);

    mediator->removeWaypoint(0, 0);

    QCOMPARE(view->pointNumber(), 1);
    QCOMPARE(view->getCoordinates(0), correct);
}

void mediatortest::deletePointInCorrect() {
    clear();
    QString name("1");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);

    mediator->removeWaypoint(0, 0);

    QCOMPARE(view->pointNumber(), 0);
}

void mediatortest::editPointCorrect() {
    clear();
    QString name("1");
    coordinates correct(0, 0, 14);
    coordinates newCorrect(0, 1, 14);

    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, correct);
    QCOMPARE(view->pointNumber(), 1);
    mediator->editWaypoint(0, 0, newCorrect);
    QCOMPARE(view->pointNumber(), 1);
    QCOMPARE(view->getCoordinates(0), newCorrect);
}

void mediatortest::editPointIncorrect() {
    clear();
    QString name("1");
    coordinates correct(0, 0, 14);
    coordinates newInCorrect(0, 99999, 0);

    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, correct);
    QCOMPARE(view->pointNumber(), 1);
    mediator->editWaypoint(0, 0, newInCorrect);
    QCOMPARE(view->pointNumber(), 1);
    QCOMPARE(view->getCoordinates(0), correct);
}

void mediatortest::fromPolyCorrect() {
    clear();
    QString name("saddas");
    QString poly("_p~iF~ps|U_ulLnnqC_mqNvxq`@");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addRouteFromPolyline(name, poly);
    QCOMPARE(view->routeNumber(), 1);
    QCOMPARE(view->pointNumber(), 3);
    QCOMPARE(view->getCoordinates(0), coordinates(38.5, -120.2));
    QCOMPARE(view->getCoordinates(1), coordinates(40.7, -120.95));
    QCOMPARE(view->getCoordinates(2), coordinates(43.252, -126.453));
}

void mediatortest::fromPolyIncorrect() {
    clear();
    QString name("saddas");
    QString poly("_safjflsfsajslf");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addRouteFromPolyline(name, poly);
    QCOMPARE(view->routeNumber(), 0);
    QCOMPARE(view->pointNumber(), 0);
}

void mediatortest::graphCorrect() {
    clear();
    QFile::remove(backupPath);

    QFile::copy("savedstate1.dat", backupPath);
    mediator->loadState();
    route rt = mediator->getRoute(1);
    QCOMPARE(rt.getNumberOfPoints(), 3);

    QCOMPARE(rt[0], coordinates(0, 2, 1));
    QCOMPARE(rt[1], coordinates(5, 2, 1));
    QCOMPARE(rt[2], coordinates(5, 3, 1));
}

void mediatortest::graphInCorrect() {
    clear();
    QString name("route");
    mediator->addSampleRoute(name);
    route rt = mediator->getRoute(0);
    QCOMPARE(rt.getNumberOfPoints(), 0);
}

void mediatortest::setupSignals() {
    connect(manager, SIGNAL(routeAdded(route&,size_t)), mediator, SLOT(addRouteToView(route&,size_t)));
    connect(manager, SIGNAL(routeRemoved(size_t)), mediator, SLOT(removeRouteFromView(size_t)));
    connect(manager, SIGNAL(waypointAdded(size_t,coordinates&,size_t)), mediator, SLOT(addWaypointToView(size_t,coordinates&,size_t)));
    connect(manager, SIGNAL(waypointRemoved(size_t,size_t)), mediator, SLOT(removeWaypointFromView(size_t,size_t)));
    connect(manager, SIGNAL(waypointEdited(size_t,coordinates&,size_t)), mediator, SLOT(editWaypointInView(size_t,coordinates&,size_t)));
}

void mediatortest::clear() {
    while (manager->size()) {
        manager->removeRoute(0);
    }
    view->clear();
}
