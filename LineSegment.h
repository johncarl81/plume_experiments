#ifndef PLUME_EXPERIMENTS_LINESEGMENT_H
#define PLUME_EXPERIMENTS_LINESEGMENT_H

#include "Point.h"
#include "Line.h"

class LineSegment {

public:
    LineSegment(const Line &line, const Point &start, const Point &end);

    LineSegment(const LineSegment& copySegment);

    ~LineSegment();

    double length();

    Line getLine();

    Point getStart();

    Point* getStartPtr();

    Point getEnd();

    Point* getEndPtr();

    friend std::ostream& operator<<(std::ostream &strm, const LineSegment &segment) {
        return strm << "start: " << segment.start << " end: " << segment.end;
    }

private:
    Line line;
    Point *start, *end;
};

#endif //PLUME_EXPERIMENTS_LINESEGMENT_H
