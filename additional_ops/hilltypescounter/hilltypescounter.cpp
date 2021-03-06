#include "hilltypescounter.h"
#include <cmath>
#include "../../route.h"

hilltypescounter::hilltypescounter() {
    heightTypes.resize(heightTypesCount);
    heightTypes.fill(0);
    lengthTypes.resize(lengthTypesCount);
    lengthTypes.fill(0);
}

QString hilltypescounter::visit(route *rm) {
    unsigned pointCount = rm->getNumberOfPoints();
    unsigned hillCount = pointCount - 1;

    for (unsigned i = 0; i < hillCount; ++i) {
        unsigned hillHeightType = getHillHeightType((*rm)[i], (*rm)[i + 1]);
        if (hillHeightType < 5) {
            heightTypes[hillHeightType]++;
            double lengthType = getHillLengthType((*rm)[i], (*rm)[i + 1]);
            lengthTypes[lengthType]++;
        }
    }

    QString text("По крутизне:\n");
    for (int i = 0; i < heightTypes.size(); ++i) {
        text += QString::number(i + 1) + ": " + QString::number(heightTypes[i]) + "\n";
    }
    text += "\nПо длине:\n";
    for (int i = 0; i < lengthTypes.size(); ++i) {
        text += QString::number(i + 1) + ": " + QString::number(lengthTypes[i]) + "\n";
    }

    return text;
}

unsigned hilltypescounter::getHillHeightType(coordinates& c0, coordinates& c1) {
    double dHeight = fabs(c1.getHeight() - c0.getHeight());
    double distance = c1.distance(c0);

    double angle = asin(dHeight / distance);

    if (angle > verySteep) {
        return 0;
    } else if (angle > steep) {
        return 1;
    } else if (angle > medium) {
        return 2;
    } else if (angle > sloping) {
        return 3;
    } else if (angle > verySloping){
        return 4;
    } else {
        return 5;
    }
}

unsigned hilltypescounter::getHillLengthType(coordinates& c0, coordinates& c1) {
    double distance = c0.distance(c1);

    if (distance > veryLong) {
        return 0;
    } else if (distance > mediumLong) {
        return 1;
    } else {
        return 2;
    }
}

extern "C" HILLTYPESCOUNTERSHARED_EXPORT  visitor* createInstance() {
   return new hilltypescounter();
}
