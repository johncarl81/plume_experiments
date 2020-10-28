#include "SearchStrategy.h"

void SearchStrategy::executeUntil(double distance) {
    for (; getMaxDistance() < distance; distanceIteration++) {
        int trials = pow(2, distanceIteration);
        for (int i = 0; i < trials; i++) {
            execute();
        }
    }
}
