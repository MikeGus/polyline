#include "routetest.h"
#include "../route.h"

#include <iostream>

void routetest::toPolyline_1() {
    route testRoute;
    testRoute.addLast(coordinates(38.5, -120.2));
    testRoute.addLast(coordinates(40.7, -120.95));
    testRoute.addLast(coordinates(43.252, -126.453));
    QCOMPARE(testRoute.generatePolyline(), polyline("_p~iF~ps|U_ulLnnqC_mqNvxq`@"));
}

void routetest::toPolyline_2() {
    route testRoute;
    testRoute.addLast(coordinates(0.0, 0.0));
    QCOMPARE(testRoute.generatePolyline(), polyline("??"));
}

void routetest::toPolyline_3() {
    route testRoute;
    testRoute.addLast(coordinates(-90, -180));
    testRoute.addLast(coordinates(0.0, 0.0));
    testRoute.addLast(coordinates(90.0, 180.0));
    QCOMPARE(testRoute.generatePolyline(), polyline("~bidP~fsia@_cidP_gsia@_cidP_gsia@"));
}

void routetest::toPolyline_4() {
    route testRoute;
    QCOMPARE(testRoute.generatePolyline(), polyline());
}

void routetest::fromPolyline_1() {
    route testRoute1,
            testRoute2(QString(""), QString(""), polyline("_p~iF~ps|U_ulLnnqC_mqNvxq`@"));
    testRoute1.addLast(coordinates(38.5, -120.2));
    testRoute1.addLast(coordinates(40.7, -120.95));
    testRoute1.addLast(coordinates(43.252, -126.453));
    QCOMPARE(testRoute1, testRoute2);
}

void routetest::fromPolyline_2() {
    route testRoute1,
            testRoute2(QString(""), QString(""), polyline("??"));
    testRoute1.addLast(coordinates(0.0, 0.0));
    QCOMPARE(testRoute1, testRoute2);
}

void routetest::fromPolyline_3() {
    route testRoute1,
            testRoute2(QString(""), QString(""), polyline("~bidP~fsia@_cidP_gsia@_cidP_gsia@"));
    testRoute1.addLast(coordinates(-90.0, -180.0));
    testRoute1.addLast(coordinates(0.0, 0.0));
    testRoute1.addLast(coordinates(90.0, 180.0));
    QCOMPARE(testRoute1, testRoute2);
}

void routetest::fromPolyline_4() {
    route testRoute1,
            testRoute2(QString(""), QString(""), polyline(""));
    QCOMPARE(testRoute1, testRoute2);
}

