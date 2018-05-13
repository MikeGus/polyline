#ifndef DELETEWAYPOINTCOMMAND_H
#define DELETEWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <QVector>
#include "../routemanager.h"

class deletewaypointcommand: public QUndoCommand {
public:
    deletewaypointcommand(routemanager* routes, size_t routeIndex,
                          QVector<coordinates>& delWaypoints, QVector<size_t>& positions,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    size_t routeIndex;
    QVector<coordinates> delWaypoints;
    QVector<size_t> positions;
};

#endif // DELETEWAYPOINTCOMMAND_H
