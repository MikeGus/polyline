#include "editwaypointcommand.h"

editwaypointcommand::editwaypointcommand(routemanager* routes, size_t routeIndex,
                                         coordinates& editWaypoint,
                                         coordinates& previousWaypoint,
                                         size_t position,
                                         QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), routeIndex(routeIndex),
    editWaypoint(editWaypoint), previousWaypoint(previousWaypoint), position(position) {
}

void editwaypointcommand::redo() {
    routes->editWaypoint(routeIndex, editWaypoint, position);
}

void editwaypointcommand::undo() {
    routes->editWaypoint(routeIndex, previousWaypoint, position);
}
