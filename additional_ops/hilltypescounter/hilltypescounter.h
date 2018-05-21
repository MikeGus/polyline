#ifndef HILLTYPESCOUNTER_H
#define HILLTYPESCOUNTER_H

#include "hilltypescounter_global.h"
#include "../../visitor.h"
#include "../../coordinates.h"
#include <QVector>
#include <cmath>

const unsigned heightTypesCount = 5;
const unsigned lengthTypesCount = 3;

const double verySteep = 35 / 180 * M_PI;
const double steep = 15 / 180 * M_PI;
const double medium = 8 / 180 * M_PI;
const double sloping = 4 / 180 * M_PI;
const double verySloping = 2 / 180 * M_PI;

const double veryLong = 500;
const double mediumLong = 50;

class HILLTYPESCOUNTERSHARED_EXPORT hilltypescounter: public visitor {

public:
    hilltypescounter();

    void visit(route* rm) override;

private:

    unsigned getHillHeightType(coordinates& c0, coordinates& c1);

    unsigned getHillLengthType(coordinates& c0, coordinates& c1);

    QVector<unsigned> heightTypes;
    QVector<unsigned> lengthTypes;
};

#endif // HILLTYPESCOUNTER_H
