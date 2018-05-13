#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include <QUndoStack>
#include <QTableWidget>
#include <QStringList>
#include "routemanager.h"

class baseview;

static const QString backupPath("savedstate.dat");

class presenter : public QObject {
    Q_OBJECT
public:
    presenter(QUndoStack* undoStack, routemanager* routeManager, baseview* view,
              QObject *parent = nullptr);

signals:
    void displayError(const char* msg);

public slots:
    void addSampleRoute(QString& name);

    void addRouteFromPolyline(QString& name, QString& poly);

    void addRouteFromFiles(QStringList& filenames);

    void removeRoute(size_t position);

    void addWaypoint(size_t selectedRoute, coordinates& newPoint);

    void editWaypoint(size_t selectedRoute, size_t selectedPoint,
                      coordinates& newPoint);

    void removeWaypoint(size_t selectedRoute, size_t selectedPoint);

    void saveState();

    void loadState();

    void addRouteToView(route& newRoute, size_t position);

    void removeRouteFromView(size_t position);

    void addWaypointToView(size_t routePosition, coordinates& waypoint,
                           size_t waypointPosition);

    void removeWaypointFromView(size_t routePosition, size_t waypointPosition);

    void editWaypointInView(size_t routePosition, coordinates& waypoint, size_t waypointPosition);

    const route& getRoute(size_t routePosition) const;

private:
    QUndoStack* undoStack;
    routemanager* routeManager;

    baseview* view;
};

#endif // PRESENTER_H
