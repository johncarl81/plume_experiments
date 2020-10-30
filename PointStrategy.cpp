#include "PointStrategy.h"

PointStrategy::PointStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region) {}

void PointStrategy::execute() {
    Point point = randomPoint();

    while (point.length() > areaRadius) {
        point = randomPoint();
    }

    if (region.inside(point)) {
        hits++;
    }
    if (!first) {
        distance += (previous - point).length();
    } else {
        first = false;
    }


    previous = point;
    samples++;

    history.push_back(point);
    distanceEstimate.emplace_back(distance, 1.0 * area.size() * hits / samples);
}

Point PointStrategy::randomPoint() {
    return Point(RandomUtil::uniform_random(-areaRadius, areaRadius),
                 RandomUtil::uniform_random(-areaRadius, areaRadius));
}

double PointStrategy::getMaxDistance() {
    return distance;
}

vector<DistanceEstimate>* PointStrategy::getDistanceEstimate() {
    return &distanceEstimate;
}


