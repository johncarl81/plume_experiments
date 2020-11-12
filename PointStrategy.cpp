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

    history.push_back(new Point(point.getX(), point.getY()));
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

void PointStrategy::optimize() {
    TSP::optimize(history);

    // Recalculate distances.
    distance = 0;
    hits = 0;
    samples = 0;
    bool firstLoop = true;
    for(int i = 0; i < history.size(); i++) {
        if(firstLoop) {
            firstLoop = false;
        } else {
            distance += ((*history[i]) - (*history[i-1])).length();
        }

        if (region.inside(*history[i])) {
            hits++;
        }
        samples++;

        distanceEstimate[i].distance = distance;
        distanceEstimate[i].estimate = 1.0 * area.size() * hits / samples;
    }
}

int PointStrategy::size() {
    return history.size();
}

PointStrategy::~PointStrategy() {
    for(Point* point : history) {
        delete point;
    }
}


