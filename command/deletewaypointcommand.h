#ifndef DELETEWAYPOINTCOMMAND_H
#define DELETEWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include "../routemanager.h"

class deletewaypointcommand: public QUndoCommand {
public:
    deletewaypointcommand(routemanager* routes, size_t routeIndex,
                          coordinates& delWaypoint, size_t position,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    size_t routeIndex;
    coordinates delWaypoint;
    size_t position;
};

#endif // DELETEWAYPOINTCOMMAND_H
