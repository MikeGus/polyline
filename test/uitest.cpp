#include "uitest.h"
#include "../testview.h"
#include "../presenter.h"
#include "../routemanager.h"
#include <QUndoStack>
#include <QUndoCommand>

uitest::uitest() {
    stack = new QUndoStack;
    view = new testview;
    manager = new routemanager;
    mediator = new presenter(stack, manager, view);
    view->setMediator(mediator);

    setupSignals();
}

uitest::~uitest() {
    delete mediator;
    delete manager;
    delete view;
    delete stack;
}

void uitest::routeListCorrect() {
    clear();
    mediator->loadState();
}

void uitest::routeListIncorrect() {
    clear();
    mediator->loadState();
}

void uitest::createRouteCorrect() {
    clear();
    QString name("1");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);
    QCOMPARE(view->routeNumber(), 1);
}

void uitest::createRouteIncorrect() {
    clear();
    QString name("saddas");
    QString poly("aflfasjlsakj");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addRouteFromPolyline(name, poly);
    QCOMPARE(view->routeNumber(), 0);
}

void uitest::deleteRouteCorrect() {
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

void uitest::deleteRouteInCorrect() {
    clear();
    QCOMPARE(view->routeNumber(), 0);
    mediator->removeRoute(0);
    QCOMPARE(view->routeNumber(), 0);
}

void uitest::createPointCorrect() {
    clear();
    QString name("1");
    coordinates correct(0, 0, 14);
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, correct);
    QCOMPARE(view->pointNumber(), 1);
}

void uitest::createPointInCorrect() {
    clear();
    QString name("1");
    coordinates incorrect(0, 1800, 14);
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);
    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, incorrect);
    QCOMPARE(view->pointNumber(), 0);
}

void uitest::deletePointCorrect() {
    clear();
    QString name("1");
    coordinates correct(0, 0, 14);
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);
    mediator->addWaypoint(0, correct);
    mediator->addWaypoint(0, correct);
    QCOMPARE(view->pointNumber(), 2);

    mediator->removeWaypoint(0, 0);

    QCOMPARE(view->pointNumber(), 1);
}

void uitest::deletePointInCorrect() {
    clear();
    QString name("1");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addSampleRoute(name);

    QCOMPARE(view->pointNumber(), 0);

    mediator->removeWaypoint(0, 0);

    QCOMPARE(view->pointNumber(), 0);
}

void uitest::editPointCorrect() {
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
}

void uitest::editPointIncorrect() {
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
}

void uitest::fromPolyCorrect() {
    clear();
    QString name("saddas");
    QString poly("_p~iF~ps|U_ulLnnqC_mqNvxq`@");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addRouteFromPolyline(name, poly);
    QCOMPARE(view->routeNumber(), 1);

}

void uitest::fromPolyIncorrect() {
    clear();
    QString name("saddas");
    QString poly("_safjflsfsajslf");
    QCOMPARE(view->routeNumber(), 0);
    mediator->addRouteFromPolyline(name, poly);
    QCOMPARE(view->routeNumber(), 0);
}

void uitest::setupSignals() {
    connect(manager, SIGNAL(routeAdded(route&,size_t)), mediator, SLOT(addRouteToView(route&,size_t)));
    connect(manager, SIGNAL(routeRemoved(size_t)), mediator, SLOT(removeRouteFromView(size_t)));
    connect(manager, SIGNAL(waypointAdded(size_t,coordinates&,size_t)), mediator, SLOT(addWaypointToView(size_t,coordinates&,size_t)));
    connect(manager, SIGNAL(waypointRemoved(size_t,size_t)), mediator, SLOT(removeWaypointFromView(size_t,size_t)));
    connect(manager, SIGNAL(waypointEdited(size_t,coordinates&,size_t)), mediator, SLOT(editWaypointInView(size_t,coordinates&,size_t)));
}

void uitest::clear() {
    while (manager->size()) {
        manager->removeRoute(0);
    }
    view->clear();
}
