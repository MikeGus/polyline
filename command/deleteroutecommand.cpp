#include "deleteroutecommand.h"

deleteroutecommand::deleteroutecommand(routemanager* routes, QVector<route>& delRoutes,
                                       QVector<size_t>& positions, QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), delRoutes(delRoutes),
    positions(positions) {
}

void deleteroutecommand::redo() {
    for (size_t& position : positions) {
        routes->removeRoute(position);
    }
}

void deleteroutecommand::undo() {
    for (size_t i = 0; i < delRoutes.size(); ++i) {
        routes->addRoute(delRoutes[i], positions[i]);
    }
}
