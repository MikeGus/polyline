#include "routemanager.h"

routemanager::routemanager(QObject *parent) : QObject(parent) {
}

void routemanager::addRoute(route& newRoute, size_t position) {
    if (routes.size() >= position) {
        routes.insert(routes.begin() + position, newRoute);
        emit routeAdded(newRoute, position);
    }
}

void routemanager::removeRoute(size_t position) {
    if (routes.size() > position) {
        routes.erase(routes.begin() + position);
        emit routeRemoved(position);
    }
}

void routemanager::addWaypoint(size_t routeIndex, coordinates& newWaypoint,
                 size_t position) {
    routes.at(routeIndex).add(newWaypoint, position);
    emit waypointAdded(routeIndex, newWaypoint, position);
}

void routemanager::removeWaypoint(size_t routeIndex, size_t position) {
    routes.at(routeIndex).remove(position);
    emit waypointRemoved(routeIndex, position);
}

void routemanager::editWaypoint(size_t routeIndex, coordinates& newWaypoint,
                                size_t position) {
    routes.at(routeIndex)[position] = newWaypoint;
    emit waypointEdited(routeIndex, newWaypoint, position);
}

route& routemanager::at(int position) {
    return routes.at(position);
}


size_t routemanager::size() {
    return routes.size();
}
