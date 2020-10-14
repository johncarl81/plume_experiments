#ifndef PLUME_EXPERIMENTS_LINESEGMENT_H
#define PLUME_EXPERIMENTS_LINESEGMENT_H

#include "Point.h"
#include "Line.h"

class LineSegment {

public:
    LineSegment(const Line &line, const Point &start, const Point &end);

    double length();

    const Line &getLine() const;

    const Point &getStart() const;

    const Point &getEnd() const;

private:
    Line line;
    Point start, end;
};

#endif //PLUME_EXPERIMENTS_LINESEGMENT_H
