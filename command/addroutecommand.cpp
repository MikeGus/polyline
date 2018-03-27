#include "addroutecommand.h"

addroutecommand::addroutecommand(std::vector<route>* routes, route& newRoute,
                                 QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), newRoute(newRoute) {
}

void addroutecommand::redo() {
    routes->push_back(newRoute);
}

void addroutecommand::undo() {
    routes->pop_back();
}
