#ifndef EDITWAYPOINTCOMMAND_H
#define EDITWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "coordinates.h"

class MainWindow;

class editwaypointcommand: public QUndoCommand {
public:
    editwaypointcommand(MainWindow* mainWindow, size_t routeIndex, coordinates& editWaypoint,
                        coordinates& previousWaypoint, size_t position,
                        QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    MainWindow* mainWindow;
    size_t routeIndex;
    coordinates editWaypoint;
    coordinates previousWaypoint;
    size_t position;
};

#endif // EDITWAYPOINTCOMMAND_H
