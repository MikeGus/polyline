#include "editwaypointcommand.h"

editwaypointcommand::editwaypointcommand(routemanager* routes, size_t routeIndex,
                                         QVector<coordinates>& editWaypoints,
                                         QVector<coordinates>& previousWaypoints,
                                         QVector<size_t>& positions,
                                         QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), routeIndex(routeIndex),
    editWaypoints(editWaypoints), previousWaypoints(previousWaypoints), positions(positions) {
}

void editwaypointcommand::redo() {
    for (int i = 0; i < editWaypoints.length(); ++i) {
        routes->editWaypoint(routeIndex, editWaypoints[i], positions[i]);
    }
}

void editwaypointcommand::undo() {
    for (int i = 0; i < editWaypoints.length(); ++i) {
        routes->editWaypoint(routeIndex, previousWaypoints[i], positions[i]);
    }
}
