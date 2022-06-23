#include "CompositePlume.h"

bool CompositePlume::crosses(LineSegment &segment) {
    return head->crosses(segment) || tail->crosses(segment);
}

bool CompositePlume::crossesEdge(LineSegment &segment) {
    return head->crossesEdge(segment) || tail->crossesEdge(segment);
}

bool CompositePlume::inside(const Point &point) {
    return head->inside(point) || tail->inside(point);
}

void CompositePlume::print(std::ostream &strm) const {
    head->print(strm);
    tail->print(strm);
}

Point CompositePlume::getCross(LineSegment &segment) {
    if(head->crossesEdge(segment)) {
        return head->getCross(segment);
    } else {
        return tail->getCross(segment);
    }
}

double CompositePlume::edgeGradient(Point& point) {
    if(head->inside(point)) {
        return tail->edgeGradient(point);
    } else {
        return head->edgeGradient(point);
    }
}