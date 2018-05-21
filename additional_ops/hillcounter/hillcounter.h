#ifndef HILLCOUNTER_H
#define HILLCOUNTER_H

#include "hillcounter_global.h"
#include <QVector>
#include "../../visitor.h"

class HILLCOUNTERSHARED_EXPORT hillcounter: public visitor {

public:
    void visit(route* rm) override;

private:
    unsigned count = 0;
};

#endif // HILLCOUNTER_H
