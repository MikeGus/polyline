#ifndef MEDIATORTEST_H
#define MEDIATORTEST_H

#include <QObject>
#include <QUndoStack>
#include <QtTest/QTest>

class presenter;
class testview;
class routemanager;

class mediatortest : public QObject {
    Q_OBJECT

public:
    mediatortest();
    ~mediatortest();

private slots:

    void routeListCorrect();

    void routeListIncorrect();

    void createRouteCorrect();

    void createRouteIncorrect();

    void deleteRouteCorrect();

    void deleteRouteInCorrect();

    void createPointCorrect();

    void createPointInCorrect();

    void deletePointCorrect();

    void deletePointInCorrect();

    void editPointCorrect();

    void editPointIncorrect();

    void fromPolyCorrect();

    void fromPolyIncorrect();

    void graphCorrect();

    void graphInCorrect();

private:
    QUndoStack* stack;
    presenter* mediator;
    testview* view;
    routemanager* manager;

    void setupSignals();

    void clear();
};

#endif // MEDIATORTEST_H
