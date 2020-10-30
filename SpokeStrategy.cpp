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

    LineSegment spokeIntersection = region.segmentIntersections(spoke);

    withinSpokeLength += spokeIntersection.length();
    distance += 2 * areaRadius;

    spokes++;

    history.push_back(spoke);
    distanceEstimate.emplace_back(distance, 1.004159225 * M_PI * pow(withinSpokeLength / spokes, 2));
}

vector<DistanceEstimate>* SpokeStrategy::getDistanceEstimate() {
    return &distanceEstimate;
}

double SpokeStrategy::getMaxDistance() {
    return distance;
}

