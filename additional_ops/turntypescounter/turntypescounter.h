#ifndef TURNTYPESCOUNTER_H
#define TURNTYPESCOUNTER_H

#include "turntypescounter_global.h"
#include "../../visitor.h"
#include "../../coordinates.h"
#include <QVector>

const unsigned turnsNumber = 8;

class TURNTYPESCOUNTERSHARED_EXPORT turntypescounter: public visitor {
public:
    turntypescounter();

    QString visit(route* rm) override;

    QVector<unsigned> turns;

private:
    unsigned checkTurnType(coordinates& c0, coordinates& c1, coordinates& c2);

    double getAngle(coordinates& c0, coordinates& c1, coordinates& c2);

    unsigned getAngleType(double angle);
};

#endif // TURNTYPESCOUNTER_H
