#ifndef HILLTYPESCOUNTER_H
#define HILLTYPESCOUNTER_H

#include "hilltypescounter_global.h"
#include "../../visitor.h"
#include "../../coordinates.h"
#include <QVector>
#include <cmath>

const unsigned heightTypesCount = 5;
const unsigned lengthTypesCount = 3;

const double verySteep = 35.0 / 180.0 * M_PI;
const double steep = 15.0 / 180.0 * M_PI;
const double medium = 8.0 / 180.0 * M_PI;
const double sloping = 4.0 / 180.0 * M_PI;
const double verySloping = 2.0 / 180.0 * M_PI;

const double veryLong = 500;
const double mediumLong = 50;

class HILLTYPESCOUNTERSHARED_EXPORT hilltypescounter: public visitor {

public:
    hilltypescounter();

    QString visit(route* rm) override;

    QVector<unsigned> heightTypes;

    QVector<unsigned> lengthTypes;

private:

    unsigned getHillHeightType(coordinates& c0, coordinates& c1);

    unsigned getHillLengthType(coordinates& c0, coordinates& c1);
};

#endif // HILLTYPESCOUNTER_H
