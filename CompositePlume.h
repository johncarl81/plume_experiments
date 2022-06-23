#ifndef PLUME_EXPERIMENTS_COMPOSITEPLUME_H
#define PLUME_EXPERIMENTS_COMPOSITEPLUME_H


#include "Plume.h"

class CompositePlume: public Plume {

public:
    CompositePlume(Plume* head, Plume* tail) : head(head), tail(tail) {};

    bool crosses(LineSegment &segment);

    bool crossesEdge(LineSegment &segment);

    bool inside(const Point &point);

    Point getCross(LineSegment &segment);

    double edgeGradient(Point& point);

private:
    Plume* head;
    Plume* tail;

    void print(std::ostream &ostream) const override;
};


#endif //PLUME_EXPERIMENTS_COMPOSITEPLUME_H
