#ifndef DELETEROUTECOMMAND_H
#define DELETEROUTECOMMAND_H

#include <QUndoCommand>
#include <QVector>
#include "../routemanager.h"

class deleteroutecommand: public QUndoCommand {
public:
    deleteroutecommand(routemanager* routes, QVector<route>& delRoutes, QVector<size_t>& positions,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    QVector<route> delRoutes;
    QVector<size_t> positions;
};

#endif // DELETEROUTECOMMAND_H
