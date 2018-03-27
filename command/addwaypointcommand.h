#ifndef ADDWAYPOINTCOMMAND_H
#define ADDWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "coordinates.h"

class addwaypointcommand: public QUndoCommand {
public:
    addwaypointcommand(std::vector<coordinates>* route, coordinates& newWaypoint,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    std::vector<coordinates>* route;
    coordinates newWaypoint;
};

#endif // ADDWAYPOINTCOMMAND_H
