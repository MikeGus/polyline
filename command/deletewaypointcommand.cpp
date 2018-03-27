#include "deletewaypointcommand.h"

deletewaypointcommand::deletewaypointcommand(std::vector<coordinates>* route,
                                          coordinates& delWaypoint, size_t position,
                                          QUndoCommand* parent):
    QUndoCommand(parent), route(route), delWaypoint(delWaypoint), position(position) {
}

void deletewaypointcommand::redo() {
    route->erase(route->begin() + position);
}

void deletewaypointcommand::undo() {
    route->insert(route->begin() + position, delWaypoint);
}
