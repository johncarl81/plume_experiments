#ifndef PLUME_EXPERIMENTS_JOHNSPOKESTRATEGY_H
#define PLUME_EXPERIMENTS_JOHNSPOKESTRATEGY_H

#include <random>
#include "Ellipse.h"
#include "SearchStrategy.h"
#include "RandomUtil.h"

using namespace std;

class JohnSpokeStrategy : public SearchStrategy {
public:
    JohnSpokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region);

    void executeUntil(long distance) override;

    double getAreaEstimate() override;

private:
    double areaRadius;
    Ellipse area, region;

    double withinSpokeLength = 0;
    double length = 0;
    long spokes = 0;
};


#endif //PLUME_EXPERIMENTS_JOHNSPOKESTRATEGY_H
