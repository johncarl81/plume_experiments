#ifndef PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
#define PLUME_EXPERIMENTS_SEARCHSTRATEGY_H

#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

class DistanceEstimate {
public:
    DistanceEstimate(double distance, double estimate) : distance(distance), estimate(estimate) {};

    bool operator<(const DistanceEstimate &first) const {
        return distance < first.distance;
    }

    bool operator>(const DistanceEstimate &first) const {
        return distance > first.distance;
    }

    double distance, estimate;
};

class SearchStrategy {

public:

    SearchStrategy() = default;

    virtual ~SearchStrategy() = default;

    virtual void execute() = 0;

    virtual double getMaxDistance() = 0;

    void executeUntil(double distance);

    double getAreaEstimate(double distance);

    virtual vector<DistanceEstimate>* getDistanceEstimate() = 0;

private:
    std::vector<DistanceEstimate>::iterator lower(std::vector<DistanceEstimate>::iterator first, std::vector<DistanceEstimate>::iterator last, double value);

    int distanceIteration = 0;
};


#endif //PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
