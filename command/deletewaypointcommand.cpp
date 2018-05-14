#include "deletewaypointcommand.h"

deletewaypointcommand::deletewaypointcommand(routemanager* routes, size_t routeIndex,
                                          QVector<coordinates>& delWaypoints, QVector<size_t>& positions,
                                          QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), routeIndex(routeIndex),
    delWaypoints(delWaypoints), positions(positions) {
}

void deletewaypointcommand::redo() {
    for (size_t& position : positions) {
        routes->removeWaypoint(routeIndex, position);
    }
}

void deletewaypointcommand::undo() {
    for (int i = 0; i < delWaypoints.length(); ++i) {
        routes->addWaypoint(routeIndex, delWaypoints[i], positions[i]);
    }
}
