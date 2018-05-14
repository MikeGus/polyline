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

    virtual void displayError(const char* msg) override;

};

#endif // TESTVIEW_H
