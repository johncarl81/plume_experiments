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
    distances.push_back(distance);
    estimate.push_back(1.0 * area.size() * hits / samples);
}

double PointStrategy::getAreaEstimate(double distance) {
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

Point PointStrategy::randomPoint() {
    return Point(RandomUtil::uniform_random(-areaRadius, areaRadius),
                 RandomUtil::uniform_random(-areaRadius, areaRadius));
}

double PointStrategy::getMaxDistance() {
    return distance;
}
