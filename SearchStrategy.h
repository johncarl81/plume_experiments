
#ifndef PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
#define PLUME_EXPERIMENTS_SEARCHSTRATEGY_H

#include <cmath>
#include <iostream>

using namespace std;

class SearchStrategy {

public:
    virtual void execute() = 0;

    virtual double getMaxDistance() = 0;

    void executeUntil(double distance);

    virtual double getAreaEstimate(double distance) = 0;

private:
    int distanceIteration = 0;
};


#endif //PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
