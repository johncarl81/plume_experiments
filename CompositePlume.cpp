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
