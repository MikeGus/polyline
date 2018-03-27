#ifndef DELETEWAYPOINTCOMMAND_H
#define DELETEWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "coordinates.h"

class deletewaypointcommand: public QUndoCommand {
public:
    deletewaypointcommand(std::vector<coordinates>* route, coordinates& delWaypoint, size_t position,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    std::vector<coordinates>* route;
    coordinates delWaypoint;
    size_t position;
};

#endif // DELETEWAYPOINTCOMMAND_H
