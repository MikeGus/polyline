#ifndef VISITOR_H
#define VISITOR_H

#include <QString>

class route;

class visitor {
public:
    virtual QString visit(route* rm) = 0;

    virtual ~visitor() = default;
};

#endif // VISITOR_H
