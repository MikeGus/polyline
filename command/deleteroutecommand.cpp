#include "deleteroutecommand.h"

deleteroutecommand::deleteroutecommand(routemanager* routes, route& delRoute,
                                       size_t position, QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), delRoute(delRoute),
    position(position) {
}

void deleteroutecommand::redo() {
    routes->removeRoute(position);
}

void deleteroutecommand::undo() {
    routes->addRoute(delRoute, position);
}
