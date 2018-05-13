#include "addroutecommand.h"

addroutecommand::addroutecommand(routemanager* routes, QVector<route>& newRoutes,
                                 QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), newRoutes(newRoutes) {
}

void addroutecommand::redo() {
    for (auto& newRoute : newRoutes) {
        routes->addRoute(newRoute, routes->size());
    }
}

void addroutecommand::undo() {
    for (int i = 0; i < newRoutes.length(); ++i) {
        routes->removeRoute(routes->size() - 1);
    }
}
