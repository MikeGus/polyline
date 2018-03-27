#include "addroutecommand.h"
#include "mainwindow.h"

addroutecommand::addroutecommand(MainWindow* mainWindow, route& newRoute,
                                 QUndoCommand* parent):
    QUndoCommand(parent), mainWindow(mainWindow), newRoute(newRoute) {
}

void addroutecommand::redo() {
    mainWindow->addRoute(newRoute);
}

void addroutecommand::undo() {
    mainWindow->removeRoute();
}
