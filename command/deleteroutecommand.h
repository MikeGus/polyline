#ifndef DELETEROUTECOMMAND_H
#define DELETEROUTECOMMAND_H

#include <QUndoCommand>
#include <vector>
#include "route.h"

class deleteroutecommand: public QUndoCommand {
public:
    deleteroutecommand(std::vector<route>* routes, route& delRoute, size_t position,
                    QUndoCommand* parent = nullptr);

    void redo() override;
    void undo() override;

private:
    std::vector<route>* routes;
    route delRoute;
    size_t position;
};

#endif // DELETEROUTECOMMAND_H
