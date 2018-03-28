#include "addroutecommand.h"

addroutecommand::addroutecommand(routemanager* routes, route& newRoute,
                                 QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), newRoute(newRoute) {
}

void addroutecommand::redo() {
    routes->addRoute(newRoute, routes->size());
}

void addroutecommand::undo() {
    routes->removeRoute(routes->size() - 1);
}
