#include <QCoreApplication>

#include <QtTest/QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "routemanagertest.h"
#include "routetest.h"

using namespace std;

int main(int argc, char *argv[]) {
//    freopen("testing.log", "w", stdout);
    QCoreApplication a(argc, argv);
    QTest::qExec(new routetest, argc, argv);
    QTest::qExec(new routemanagertest, argc, argv);

    return 0;
}
