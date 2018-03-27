#ifndef EDITWAYPOINTCOMMAND_H
#define EDITWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "coordinates.h"

class editwaypointcommand: public QUndoCommand {
public:
    editwaypointcommand(std::vector<coordinates>* route, coordinates& editWaypoint,
                        coordinates& previousWaypoint, size_t position,
                        QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    std::vector<coordinates>* route;
    coordinates editWaypoint;
    coordinates previousWaypoint;
    size_t position;
};

#endif // EDITWAYPOINTCOMMAND_H
