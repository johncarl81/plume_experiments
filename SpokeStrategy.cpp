#include "SpokeStrategy.h"


SpokeStrategy::SpokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region),
                                                                                              previous(Point(0,0)){}

void SpokeStrategy::execute() {
    double angle = 2.0 * M_PI * RandomUtil::uniform_random();

    Point end = Point(cos(angle) * areaRadius, sin(angle) * areaRadius);
    Point center = Point(0, 0);
    Line line = Line::buildByPointAndAngle(center, angle);

    LineSegment spoke = LineSegment(line, center, end);

    LineSegment spokeIntersection = region.segmentIntersections(spoke);

    withinSpokeLength += spokeIntersection.length();
    distance += areaRadius;
    if (!first) {
        distance += (spoke.getStart() - previous).length();
    } else {
        first = false;
    }
    previous = spoke.getEnd();

    spokes++;

    history.push_back(new LineSegment(spoke));
    distanceEstimate.emplace_back(distance, 1.004159225 * M_PI * pow(withinSpokeLength / spokes, 2));
}

vector<DistanceEstimate>* SpokeStrategy::getDistanceEstimate() {
    return &distanceEstimate;
}

double SpokeStrategy::getMaxDistance() {
    return distance;
}

int SpokeStrategy::size() {
    return history.size();
}

void SpokeStrategy::optimize() {
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

SpokeStrategy::~SpokeStrategy() {

    for(LineSegment* edge : history) {
        delete edge;
    }
}
