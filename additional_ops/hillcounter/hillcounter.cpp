#include "hillcounter.h"
#include "../../route.h"
#include <cmath>

void hillcounter::visit(route* rm) {
    unsigned pointCount = rm->getNumberOfPoints();
    if (pointCount < 2) {
        return;
    }

    unsigned roadCount = pointCount - 1;
    for (unsigned i = 0; i < roadCount; ++i) {
        double distance = (*rm)[i].distance((*rm)[i + 1]);
        double dHeight = fabs((*rm)[i].getHeight() - (*rm)[i + 1].getHeight());

        double bigHillAngle = 15 / 180 * M_PI;
        if (atan(dHeight / distance) > bigHillAngle) {
            count++;
        }
    }
}
