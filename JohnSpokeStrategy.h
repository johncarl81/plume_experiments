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

    void execute() override;

    double getMaxDistance() override;

    double getAreaEstimate(double distance) override;

private:
    double areaRadius;
    Ellipse area, region;

    vector<LineSegment> history;
    vector<double> distances;
    vector<double> estimate;

    double withinSpokeLength = 0;
    double spokeArea = 0;
    double distance = 0;
    int spokes = 0;
};


#endif //PLUME_EXPERIMENTS_JOHNSPOKESTRATEGY_H
