#ifndef TESTVIEW_H
#define TESTVIEW_H

#include "baseview.h"
#include "routemanager.h"
#include "presenter.h"
#include <QUndoStack>
#include <QTableWidget>

class testview : public baseview
{
public:
    explicit testview(QWidget* parent = 0);
    ~testview();

public slots:
//    virtual size_t getSelectedRoute() const = 0;

//    virtual size_t getSelectedWaypoint() const = 0;

//    virtual size_t getRouteCount() const = 0;

//    virtual size_t getWaypointCount() const = 0;

//    virtual QList<route> getRoutes() const = 0;

//    virtual QList<coordinates> getWaypoints() const = 0;

    virtual void displayError(const char* msg) override;

};

#endif // TESTVIEW_H
