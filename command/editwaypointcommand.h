#ifndef EDITWAYPOINTCOMMAND_H
#define EDITWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include "../routemanager.h"

class editwaypointcommand: public QUndoCommand {
public:
    editwaypointcommand(routemanager* routes, size_t routeIndex, coordinates& editWaypoint,
                        coordinates& previousWaypoint, size_t position,
                        QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    size_t routeIndex;
    coordinates editWaypoint;
    coordinates previousWaypoint;
    size_t position;
};

#endif // EDITWAYPOINTCOMMAND_H
