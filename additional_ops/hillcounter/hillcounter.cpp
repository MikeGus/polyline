#include "hillcounter.h"
#include "../../route.h"
#include <cmath>

QString hillcounter::visit(route* rm) {
    unsigned pointCount = rm->getNumberOfPoints();
    if (pointCount < 2) {
        return QString();
    }

    unsigned roadCount = pointCount - 1;
    for (unsigned i = 0; i < roadCount; ++i) {
        double distance = (*rm)[i].distance((*rm)[i + 1]);
        double dHeight = fabs((*rm)[i].getHeight() - (*rm)[i + 1].getHeight());

        double bigHillAngle = 15.0 / 180.0 * M_PI;
        if (asin(dHeight / distance) > bigHillAngle) {
            count++;
        }
    }
    QString result("Количество: ");
    result += QString::number(count) + "\n";

    return result;
}

extern "C" HILLCOUNTERSHARED_EXPORT  visitor* createInstance() {
   return new hillcounter();
}

