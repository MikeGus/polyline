#include <QCoreApplication>

#include <QtTest/QTest>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "routemanagertest.h"
#include "routetest.h"
#include "mediatortest.h"

using namespace std;

int main(int argc, char *argv[]) {
//    freopen("testing.log", "w", stdout);
    QApplication a(argc, argv);
    QTest::qExec(new routetest, argc, argv);
    QTest::qExec(new routemanagertest, argc, argv);
    QTest::qExec(new mediatortest, argc, argv);

    return 0;
}
