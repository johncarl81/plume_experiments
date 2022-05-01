#ifndef PLUME_EXPERIMENTS_LINESEGMENT_H
#define PLUME_EXPERIMENTS_LINESEGMENT_H

#include "Point.h"
#include "Line.h"

class LineSegment {

public:
    LineSegment(Point &start, Point &end);

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
        return strm << "Line " << *segment.start << " " << *segment.end << std::endl;
    }

private:
    Line line;
    Point *start, *end;
};

#endif //PLUME_EXPERIMENTS_LINESEGMENT_H
