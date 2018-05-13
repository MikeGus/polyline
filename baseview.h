#ifndef BASEVIEW_H
#define BASEVIEW_H

#include <QMainWindow>
#include "route.h"
#include "coordinates.h"
#include "routemanager.h"
#include <QUndoStack>
#include "presenter.h"
#include <QTextBrowser>
#include "qcustomplot/qcustomplot.h"

class baseview : public QMainWindow
{
    Q_OBJECT
public:
    explicit baseview(QWidget *parent = nullptr);

public slots:

    virtual void addRoute(route& newRoute, size_t position);

    virtual void addWaypoint(size_t routeIndex, coordinates& waypoint, size_t position);

    virtual void removeRoute(size_t position);

    virtual void removeWaypoint(size_t routeIndex, size_t position);

    virtual void editWaypoint(size_t routeIndex,coordinates& editWaypoint,
                      size_t position);

    virtual void displayError(const char* msg) = 0;

protected slots:

    void updateRouteStats(size_t selectedRow);

    void updateCurrentRoute(size_t selectedRow);

    size_t getSelectedRow(const QTableWidget* table) const;

    void updatePlot(size_t selectedRow);

protected:
    presenter* mediator;

    QTableWidget* routeTableWidget;
    QTableWidget* pointTableWidget;
    QTextBrowser* polylineText;

    QCustomPlot* plot;
};

#endif // BASEVIEW_H
