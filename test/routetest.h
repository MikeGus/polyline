#ifndef ROUTETEST_H
#define ROUTETEST_H

#include <QObject>
#include <QtTest/QTest>

class routetest : public QObject {
    Q_OBJECT

private slots:
    void toPolyline_1();

    void toPolyline_2();

    void toPolyline_3();

    void toPolyline_4();

    void fromPolyline_1();

    void fromPolyline_2();

    void fromPolyline_3();

    void fromPolyline_4();
};

#endif // ROUTETEST_H
