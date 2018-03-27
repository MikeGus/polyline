#include "deletewaypointcommand.h"
#include "mainwindow.h"

deletewaypointcommand::deletewaypointcommand(MainWindow* mainWindow, size_t routeIndex,
                                          coordinates& delWaypoint, size_t position,
                                          QUndoCommand* parent):
    QUndoCommand(parent), mainWindow(mainWindow), routeIndex(routeIndex),
    delWaypoint(delWaypoint), position(position) {
}

void deletewaypointcommand::redo() {
    mainWindow->removeWaypoint(routeIndex, position);
}

void deletewaypointcommand::undo() {
    mainWindow->addWaypoint(routeIndex, delWaypoint, position);
}
