#include "deleteroutecommand.h"
#include "mainwindow.h"

deleteroutecommand::deleteroutecommand(MainWindow* mainWindow, route& delRoute,
                                       size_t position, QUndoCommand* parent):
    QUndoCommand(parent), mainWindow(mainWindow), delRoute(delRoute),
    position(position) {
}

void deleteroutecommand::redo() {
    mainWindow->removeRoute(position);
}

void deleteroutecommand::undo() {
    mainWindow->addRoute(delRoute, position);
}
