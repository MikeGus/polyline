#include "deleteroutecommand.h"

deleteroutecommand::deleteroutecommand(std::vector<route>* routes, route& delRoute,
                                       size_t position, QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), delRoute(delRoute), position(position) {
}

void deleteroutecommand::redo() {
    routes->erase(routes->begin() + position);
}

void deleteroutecommand::undo() {
    routes->insert(routes->begin() + position, delRoute);
}
