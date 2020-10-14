#include "JohnSpokeStrategy.h"

JohnSpokeStrategy::JohnSpokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region) {}

void JohnSpokeStrategy::executeUntil(long distance) {
    while(length < distance) {
        double angle = 2.0 * M_PI * RandomUtil::uniform_random();

        Point end = Point(cos(angle) * areaRadius, sin(angle) * areaRadius);
        Point center = Point(0, 0);
        Line line = Line::buildByPointAndAngle(center, angle);

        LineSegment spoke = LineSegment(line, center, end);

        LineSegment spokeintersection = region.segmentIntersections(spoke);

        withinSpokeLength += spokeintersection.length() * spokeintersection.length() * M_PI;
        length += spoke.length() * spoke.length() * M_PI;

        spokes++;
    }
}

double JohnSpokeStrategy::getAreaEstimate() {
    return area.size() * withinSpokeLength / length;
}
