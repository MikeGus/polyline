#ifndef DELETEROUTECOMMAND_H
#define DELETEROUTECOMMAND_H

#include <QUndoCommand>
#include "routemanager.h"

class deleteroutecommand: public QUndoCommand {
public:
    deleteroutecommand(routemanager* routes, route& delRoute, size_t position,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    route delRoute;
    size_t position;
};

#endif // DELETEROUTECOMMAND_H
