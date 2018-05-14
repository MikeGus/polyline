#ifndef UITEST_H
#define UITEST_H

#include <QObject>
#include <QUndoStack>
#include <QtTest/QTest>

class presenter;
class testview;
class routemanager;

class uitest : public QObject {
    Q_OBJECT

public:
    uitest();
    ~uitest();

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

private:
    QUndoStack* stack;
    presenter* mediator;
    testview* view;
    routemanager* manager;

    void setupSignals();

    void clear();
};

#endif // UITEST_H
