#include "JohnSpokeStrategy.h"

JohnSpokeStrategy::JohnSpokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region) {}

void JohnSpokeStrategy::execute() {
    double angle = RandomUtil::random_angle();

    Point end = Point(cos(angle) * areaRadius, sin(angle) * areaRadius);
    Point center = Point(0, 0);
    Line line = Line::buildByPointAndAngle(center, angle);

    LineSegment spoke = LineSegment(line, center, end);

    LineSegment spokeintersection = region.segmentIntersections(spoke);

    withinSpokeLength += spokeintersection.length() * spokeintersection.length() * M_PI;
    spokeArea += spoke.length() * spoke.length() * M_PI;

    distance += 2 * areaRadius;

    spokes++;

    history.push_back(spoke);
    distances.push_back(distance);
    estimate.push_back(area.size() * withinSpokeLength / spokeArea);
}

double JohnSpokeStrategy::getAreaEstimate(double distance) {
    for(int i = 0; i < distances.size(); i++) {
        if(distances.at(i) == distance) {
            return estimate.at(i);
        }
        if(distances.at(i) > distance) {
            if(i > 0) {
                return estimate.at(i - 1);
            }
        }
    }
    return -1;
}

double JohnSpokeStrategy::getMaxDistance() {
    return distance;
}
