#include "deletewaypointcommand.h"

deletewaypointcommand::deletewaypointcommand(routemanager* routes, size_t routeIndex,
                                          coordinates& delWaypoint, size_t position,
                                          QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), routeIndex(routeIndex),
    delWaypoint(delWaypoint), position(position) {
}

void deletewaypointcommand::redo() {
    routes->removeWaypoint(routeIndex, position);
}

void deletewaypointcommand::undo() {
    routes->addWaypoint(routeIndex, delWaypoint, position);
}
