#ifndef ADDROUTECOMMAND_H
#define ADDROUTECOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "route.h"

class addroutecommand: public QUndoCommand {
public:
    addroutecommand(std::vector<route>* routes, route& newRoute,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    std::vector<route>* routes;
    route newRoute;
};

#endif // ADDROUTECOMMAND_H
