//
// Created by john on 10/15/20.
//

#include "SearchStrategy.h"

void SearchStrategy::executeUntil(double distance) {
    while (getDistance() < distance) {
        execute();
    }
}
