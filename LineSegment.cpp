#include "LineSegment.h"

LineSegment::LineSegment(const Line &line, const Point &start, const Point &end) : line(line), start(start), end(end) {}

double LineSegment::length() {
    return (end - start).length();
}

Line LineSegment::getLine() {
    return line;
}

Point LineSegment::getStart() {
    return start;
}

Point LineSegment::getEnd() {
    return end;
}
