#include "turntypescounter.h"
#include "../../route.h"
#include <cmath>

turntypescounter::turntypescounter() {
    turns.resize(turnsNumber);
    turns.fill(0);
}

void turntypescounter::visit(route* rm) {
    unsigned pointCount = rm->getNumberOfPoints();
    if (pointCount < 3) {
        return;
    }
    unsigned turnsCount = pointCount - 2;

    for (unsigned i = 0; i < turnsCount; ++i) {
        unsigned turnType = checkTurnType((*rm)[i], (*rm)[i + 1], (*rm)[i + 2]);
        turns[turnType]++;
    }
}

unsigned turntypescounter::checkTurnType(coordinates &c0, coordinates &c1, coordinates &c2) {
    return getAngleType(getAngle(c0, c1, c2));
}

double turntypescounter::getAngle(coordinates &c0, coordinates &c1, coordinates &c2) {
    double distance0 = c1.distance(c0);
    double distance1 = c2.distance(c1);
    double distance2 = c0.distance(c2);

    double cos_angle_top = distance0 * distance0 + distance1 * distance1 - distance2 * distance2;
    double cos_angle_bottom = 2 * distance0 * distance1;

    return acos(cos_angle_top / cos_angle_bottom);
}

unsigned turntypescounter::getAngleType(double angle) {
    double degree = angle / M_PI * 180;
    if (degree <= 5.5) {
        return 0;
    } else if (degree <= 11.0) {
        return 1;
    } else if (degree <= 33.5) {
        return 2;
    } else if (degree <= 56.0) {
        return 3;
    } else if (degree <= 78.5) {
        return 4;
    } else if (degree <= 90.0) {
        return 5;
    } else if (degree <= 135) {
        return 6;
    } else {
        return 7;
    }
}

extern "C" TURNTYPESCOUNTERSHARED_EXPORT  turntypescounter* create_turnTypesCounter() {
   return new turntypescounter();
}
