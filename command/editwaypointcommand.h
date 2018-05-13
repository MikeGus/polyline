#ifndef EDITWAYPOINTCOMMAND_H
#define EDITWAYPOINTCOMMAND_H

#include <QUndoCommand>
#include <QVector>
#include "../routemanager.h"

class editwaypointcommand: public QUndoCommand {
public:
    editwaypointcommand(routemanager* routes, size_t routeIndex, QVector<coordinates>& editWaypoints,
                        QVector<coordinates>& previousWaypoints, QVector<size_t>& positions,
                        QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    size_t routeIndex;
    QVector<coordinates> editWaypoints;
    QVector<coordinates> previousWaypoints;
    QVector<size_t> positions;
};

#endif // EDITWAYPOINTCOMMAND_H
