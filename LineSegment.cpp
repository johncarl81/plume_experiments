#include "LineSegment.h"

LineSegment::LineSegment(const Line &line, const Point &start, const Point &end) : line(line), start(start), end(end) {}

double LineSegment::length() {
    return (end - start).length();
}

const Line &LineSegment::getLine() const {
    return line;
}

const Point &LineSegment::getStart() const {
    return start;
}

const Point &LineSegment::getEnd() const {
    return end;
}
