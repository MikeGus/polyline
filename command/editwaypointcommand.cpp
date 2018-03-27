#include "editwaypointcommand.h"

editwaypointcommand::editwaypointcommand(std::vector<coordinates>* route, coordinates& editWaypoint,
                    coordinates& previousWaypoint, size_t position,
                                         QUndoCommand* parent):
    QUndoCommand(parent), route(route), editWaypoint(editWaypoint),
    previousWaypoint(previousWaypoint), position(position) {
}

void editwaypointcommand::redo() {
    route->emplace(route->begin() + position, editWaypoint);
}

void editwaypointcommand::undo() {
    route->emplace(route->begin() + position, previousWaypoint);
}
