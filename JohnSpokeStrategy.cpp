#include "JohnSpokeStrategy.h"

JohnSpokeStrategy::JohnSpokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region),
                                                                                              previous(Point(0, 0)){}

void JohnSpokeStrategy::execute() {
    double angle = RandomUtil::random_angle();

    Point end = Point(cos(angle) * areaRadius, sin(angle) * areaRadius);
    Point center = Point(0, 0);
    Line line = Line::buildByPointAndAngle(center, angle);

    LineSegment spoke = LineSegment(line, center, end);

    LineSegment spokeIntersection = region.segmentIntersections(spoke);

    withinSpokeLength += spokeIntersection.length() * spokeIntersection.length() * M_PI;

    distance += areaRadius;
    if (!first) {
        distance += (spoke.getStart() - previous).length();
    } else {
        first = false;
    }
    previous = spoke.getEnd();

    spokes++;

    history.push_back(new LineSegment(spoke));
    distanceEstimate.emplace_back(distance, withinSpokeLength / spokes);
}

vector<DistanceEstimate>* JohnSpokeStrategy::getDistanceEstimate() {
    return &distanceEstimate;
}

double JohnSpokeStrategy::getMaxDistance() {
    return distance;
}

int JohnSpokeStrategy::size() {
    return history.size();
}

void JohnSpokeStrategy::optimize() {
    TSP::optimize(history);

    spokes = 0;
    withinSpokeLength = 0;
    first = true;
    distance = 0;
    for(int i = 0; i < history.size(); i++) {

        LineSegment* spoke = history[i];
        LineSegment spokeIntersection = region.segmentIntersections(*spoke);

        withinSpokeLength += spokeIntersection.length() * spokeIntersection.length() * M_PI;

        distance += areaRadius;
        if (!first) {
            distance += (spoke->getStart() - previous).length();
        } else {
            first = false;
        }
        previous = spoke->getEnd();

        spokes++;

        distanceEstimate[i].distance = distance;
        distanceEstimate[i].estimate = withinSpokeLength / spokes;
    }
}

JohnSpokeStrategy::~JohnSpokeStrategy() {

    for(LineSegment* edge : history) {
        delete edge;
    }
}

