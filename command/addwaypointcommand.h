#ifndef ADDWAYPOINTCOMMAND_H
#define ADDWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "routemanager.h"

class addwaypointcommand: public QUndoCommand {
public:
    addwaypointcommand(routemanager* routes, size_t routeIndex, coordinates& newWaypoint,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    size_t routeIndex;
    coordinates newWaypoint;
};

#endif // ADDWAYPOINTCOMMAND_H
