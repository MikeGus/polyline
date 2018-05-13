#ifndef ADDROUTECOMMAND_H
#define ADDROUTECOMMAND_H

#include <QUndoCommand>
#include <QVector>
#include "../routemanager.h"

class addroutecommand: public QUndoCommand {
public:
    addroutecommand(routemanager* routes, QVector<route>& newRoutes,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    QVector<route> newRoutes;
};

#endif // ADDROUTECOMMAND_H
