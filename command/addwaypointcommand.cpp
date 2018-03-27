#include "addwaypointcommand.h"

addwaypointcommand::addwaypointcommand(std::vector<coordinates>* route,
                                       coordinates& newWaypoint,
                                       QUndoCommand* parent):
    QUndoCommand(parent), route(route), newWaypoint(newWaypoint) {
}

void addwaypointcommand::redo() {
    route->push_back(newWaypoint);
}

void addwaypointcommand::undo() {
    route->pop_back();
}
