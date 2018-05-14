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

    void setMediator(presenter* mediator);

    int pointNumber();

    int routeNumber();

    route getRoute(int position);

    coordinates getCoordinates(int position);

    void clear();

    virtual void displayError(const char* msg) override;

};

#endif // TESTVIEW_H
