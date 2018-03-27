#include "addwaypointcommand.h"
#include "mainwindow.h"

addwaypointcommand::addwaypointcommand(MainWindow* mainWindow, size_t routeIndex,
                                       coordinates& newWaypoint,
                                       QUndoCommand* parent):
    QUndoCommand(parent), mainWindow(mainWindow), routeIndex(routeIndex),
    newWaypoint(newWaypoint) {
}

void addwaypointcommand::redo() {
    mainWindow->addWaypoint(routeIndex, newWaypoint);
}

void addwaypointcommand::undo() {
    mainWindow->removeWaypoint(routeIndex);
}
