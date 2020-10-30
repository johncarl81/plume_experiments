#include "SearchStrategy.h"

void SearchStrategy::executeUntil(double distance) {
    for (; getMaxDistance() < distance; distanceIteration++) {
        int trials = pow(2, distanceIteration);
        for (int i = 0; i < trials; i++) {
            execute();
        }
    }
}

double SearchStrategy::getAreaEstimate(double distance) {
    vector<DistanceEstimate>* distanceEstimate = getDistanceEstimate();
    auto boundIter = std::lower_bound(distanceEstimate->begin(), distanceEstimate->end(), DistanceEstimate(distance, 0));

    int boundIndex = boundIter - distanceEstimate->begin();
    if(boundIndex == 0) {
        if(distanceEstimate->size() > boundIndex && distanceEstimate->at(boundIndex).distance == distance) {
            return distanceEstimate->at(boundIndex).estimate;
        } else {
            return -1;
        }
    } else if (boundIndex < distanceEstimate->size()) {
        return distanceEstimate->at(boundIndex - 1).estimate;
    } else {
        cout << "Error?" << endl;
    }
    return -1;
}

