#ifndef PLUME_EXPERIMENTS_RANDOMUTIL_H
#define PLUME_EXPERIMENTS_RANDOMUTIL_H

#include <random>

using namespace std;

class RandomUtil {

public:
    static double uniform_random();

    static double uniform_random(double min, double max);

    static double random_angle();
};


#endif //PLUME_EXPERIMENTS_RANDOMUTIL_H
