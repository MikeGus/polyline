#ifndef ROUTEMANAGERTEST_H
#define ROUTEMANAGERTEST_H

#include <QObject>
#include <QtTest/QTest>

class routemanagertest : public QObject {
    Q_OBJECT

private slots:
    void routeDelete_1();

    void routeDelete_2();

    void routeDelete_3();
};

#endif // ROUTEMANAGERTEST_H
