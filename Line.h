#ifndef PLUME_EXPERIMENTS_LINE_H
#define PLUME_EXPERIMENTS_LINE_H

#include "Point.h"

class Line {

public:
    Line(double m, double b);

    static Line buildByPoints(Point& start, Point& end);

    static Line buildByPointAndAngle(Point& start, double angle);

    double getM() const;

    double getB() const;

private:
    double m, b;
};

#endif //PLUME_EXPERIMENTS_LINE_H
