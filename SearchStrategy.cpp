#include "SearchStrategy.h"

void SearchStrategy::executeUntil(double distance) {
    for (; getMaxDistance() < distance; distanceIteration++) {
        int trials = pow(2, distanceIteration);
        for (int i = 0; i < trials; i++) {
            execute();
        }
    }
}

std::vector<DistanceEstimate>::iterator SearchStrategy::lower(std::vector<DistanceEstimate>::iterator first, std::vector<DistanceEstimate>::iterator last, double value)
{
    std::vector<DistanceEstimate>::iterator it;
    std::iterator_traits<std::vector<DistanceEstimate>::iterator>::difference_type count, step;
    count = std::distance(first, last);

    while (count > 0) {
        it = first;
        step = count / 2;
        std::advance(it, step);
        if ((*it).distance < value) {
            first = ++it;
            count -= step + 1;
        }
        else
            count = step;
    }
    return first;
}

double SearchStrategy::getAreaEstimate(double distance) {
    vector<DistanceEstimate>* distanceEstimate = getDistanceEstimate();
    auto boundIter = lower(distanceEstimate->begin(), distanceEstimate->end(), distance);

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

