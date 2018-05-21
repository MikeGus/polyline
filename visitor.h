#ifndef VISITOR_H
#define VISITOR_H

class route;

class visitor {
public:
    virtual void visit(route* rm) = 0;

    virtual ~visitor() = default;
};

#endif // VISITOR_H
