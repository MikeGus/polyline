#ifndef ADDWAYPOINTCOMMAND_H
#define ADDWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <QVector>
#include "../routemanager.h"

class addwaypointcommand: public QUndoCommand {
public:
    addwaypointcommand(routemanager* routes, size_t routeIndex, QVector<coordinates>& newWaypoints,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    size_t routeIndex;
    QVector<coordinates> newWaypoints;
};

#endif // ADDWAYPOINTCOMMAND_H
