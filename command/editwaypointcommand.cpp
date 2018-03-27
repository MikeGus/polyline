#include "editwaypointcommand.h"
#include "mainwindow.h"

editwaypointcommand::editwaypointcommand(MainWindow* mainWindow, size_t routeIndex,
                                         coordinates& editWaypoint,
                                         coordinates& previousWaypoint,
                                         size_t position,
                                         QUndoCommand* parent):
    QUndoCommand(parent), mainWindow(mainWindow), routeIndex(routeIndex),
    editWaypoint(editWaypoint), previousWaypoint(previousWaypoint), position(position) {
}

void editwaypointcommand::redo() {
    mainWindow->editWaypoint(routeIndex, editWaypoint, position);
}

void editwaypointcommand::undo() {
    mainWindow->editWaypoint(routeIndex, previousWaypoint, position);
}
