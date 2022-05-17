#include "CompositePlume.h"

bool CompositePlume::crosses(LineSegment &segment) {
    return head->crosses(segment) || tail->crosses(segment);
}

bool CompositePlume::inside(const Point &point) {
    return head->inside(point) || tail->inside(point);
}

void CompositePlume::print(std::ostream &strm) const {
    head->print(strm);
    tail->print(strm);
}

Point CompositePlume::getCross(LineSegment &segment) {
    if(tail->crosses(segment)) {
        return tail->getCross(segment);
    } else {
        return head->getCross(segment);
    }
}