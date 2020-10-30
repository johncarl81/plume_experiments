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
    distanceEstimate.emplace_back(distance, area.size() * withinSpokeLength / spokeArea);
}

vector<DistanceEstimate>* JohnSpokeStrategy::getDistanceEstimate() {
    return &distanceEstimate;
}

double JohnSpokeStrategy::getMaxDistance() {
    return distance;
}

