#include "addwaypointcommand.h"

addwaypointcommand::addwaypointcommand(routemanager* routes, size_t routeIndex,
                                       coordinates& newWaypoint,
                                       QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), routeIndex(routeIndex),
    newWaypoint(newWaypoint) {
}

void addwaypointcommand::redo() {
    routes->addWaypoint(routeIndex, newWaypoint,
                        routes->at(routeIndex).getNumberOfPoints());
}

void addwaypointcommand::undo() {
    routes->removeWaypoint(routeIndex, routes->at(routeIndex).getNumberOfPoints() - 1);
}
