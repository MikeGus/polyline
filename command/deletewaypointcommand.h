#ifndef DELETEWAYPOINTCOMMAND_H
#define DELETEWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "coordinates.h"

class MainWindow;

class deletewaypointcommand: public QUndoCommand {
public:
    deletewaypointcommand(MainWindow* mainWindow, size_t routeIndex,
                          coordinates& delWaypoint, size_t position,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    MainWindow* mainWindow;
    size_t routeIndex;
    coordinates delWaypoint;
    size_t position;
};

#endif // DELETEWAYPOINTCOMMAND_H
