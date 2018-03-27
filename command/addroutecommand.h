#ifndef ADDROUTECOMMAND_H
#define ADDROUTECOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "route.h"

class MainWindow;

class addroutecommand: public QUndoCommand {
public:
    addroutecommand(MainWindow* mainWindow, route& newRoute,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:

    MainWindow* mainWindow;
    route newRoute;
};

#endif // ADDROUTECOMMAND_H
