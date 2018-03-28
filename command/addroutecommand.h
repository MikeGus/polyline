#ifndef ADDROUTECOMMAND_H
#define ADDROUTECOMMAND_H

#include <QUndoCommand>
#include "routemanager.h"

class addroutecommand: public QUndoCommand {
public:
    addroutecommand(routemanager* routes, route& newRoute,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    routemanager* routes;
    route newRoute;
};

#endif // ADDROUTECOMMAND_H
