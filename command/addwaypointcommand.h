#ifndef ADDWAYPOINTCOMMAND_H
#define ADDWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "coordinates.h"

class MainWindow;

class addwaypointcommand: public QUndoCommand {
public:
    addwaypointcommand(MainWindow* mainWindow, size_t routeIndex, coordinates& newWaypoint,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    MainWindow* mainWindow;
    size_t routeIndex;
    coordinates newWaypoint;
};

#endif // ADDWAYPOINTCOMMAND_H
