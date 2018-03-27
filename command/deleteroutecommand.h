#ifndef DELETEROUTECOMMAND_H
#define DELETEROUTECOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "route.h"

class MainWindow;

class deleteroutecommand: public QUndoCommand {
public:
    deleteroutecommand(MainWindow* mainWindow, route& delRoute, size_t position,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    MainWindow* mainWindow;
    route delRoute;
    size_t position;
};

#endif // DELETEROUTECOMMAND_H
