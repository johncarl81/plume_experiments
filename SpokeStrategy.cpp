#include "SpokeStrategy.h"


SpokeStrategy::SpokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region) {}

void SpokeStrategy::execute() {
    double angle = 2.0 * M_PI * RandomUtil::uniform_random();

    Point end = Point(cos(angle) * areaRadius, sin(angle) * areaRadius);
    Point center = Point(0, 0);
    Line line = Line::buildByPointAndAngle(center, angle);

    LineSegment spoke = LineSegment(line, center, end);

    LineSegment spokeintersection = region.segmentIntersections(spoke);

    withinSpokeLength += spokeintersection.length();
    distance += 2 * areaRadius;

    spokes++;

    history.push_back(spoke);
    distances.push_back(distance);
    estimate.push_back(1.004159225 * M_PI * pow(withinSpokeLength / spokes, 2));
}

double SpokeStrategy::getAreaEstimate(double distance) {
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

double SpokeStrategy::getMaxDistance() {
    return distance;
}
