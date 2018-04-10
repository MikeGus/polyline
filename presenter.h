#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include <QUndoStack>
#include <QTableWidget>
#include "routemanager.h"

static const QString backupPath("savedstate.dat");

class presenter : public QObject {
    Q_OBJECT
public:
    presenter(QUndoStack* undoStack, routemanager* routeManager, QObject *parent = nullptr);

signals:
    void displayError(const char* msg);

public slots:
    void addSampleRoute(QString& name);

    void addRouteFromPolyline(QString& name, QString& poly);

    void addRouteFromFile(QString& filename);

    void removeRoute(size_t position);

    void addWaypoint(size_t selectedRoute, coordinates& newPoint);

    void editWaypoint(size_t selectedRoute, size_t selectedPoint,
                      coordinates& newPoint);

    void removeWaypoint(size_t selectedRoute, size_t selectedPoint);

    void saveState();

    void loadState();

private:
    QUndoStack* undoStack;
    routemanager* routeManager;
};

#endif // PRESENTER_H
