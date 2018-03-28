#ifndef ROUTEMANAGER_H
#define ROUTEMANAGER_H

#include <QObject>
#include "route.h"

class routemanager : public QObject {
    Q_OBJECT
public:
    routemanager(QObject *parent = nullptr);

    void addRoute(route& newRoute,  size_t position);

    void removeRoute(size_t position);

    void addWaypoint(size_t routeIndex,  coordinates& newWaypoint,
                      size_t position);

    void removeWaypoint(size_t routeIndex,  size_t position);

    void editWaypoint(size_t routeIndex,  coordinates& newWaypoint,
                       size_t position);

    route& at(int position);

    size_t size() ;

signals:
    void routeAdded(route& newRoute,  size_t position);

    void routeRemoved(size_t position);

    void waypointAdded(size_t routeIndex,  coordinates& newWaypoint,
                        size_t position);

    void waypointRemoved(size_t routeIndex,  size_t position);

    void waypointEdited(size_t routeIndex,  coordinates& newWaypoint,
                         size_t position);

private:
    std::vector<route> routes;
};

#endif // ROUTEMANAGER_H
