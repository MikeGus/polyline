#include "addwaypointcommand.h"

addwaypointcommand::addwaypointcommand(routemanager* routes, size_t routeIndex,
                                       QVector<coordinates>& newWaypoint,
                                       QUndoCommand* parent):
    QUndoCommand(parent), routes(routes), routeIndex(routeIndex),
    newWaypoints(newWaypoint) {
}

void addwaypointcommand::redo() {
    for (auto& newWaypoint : newWaypoints) {
        routes->addWaypoint(routeIndex, newWaypoint,
                            routes->at(routeIndex).getNumberOfPoints());
    }
}

void addwaypointcommand::undo() {
    for (int i = 0; i < newWaypoints.length(); ++i) {
        routes->removeWaypoint(routeIndex, routes->at(routeIndex).getNumberOfPoints() - 1);
    }
}
