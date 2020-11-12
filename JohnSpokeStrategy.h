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

    ~JohnSpokeStrategy() override = default;

    void execute() override;

    double getMaxDistance() override;

    vector<DistanceEstimate>* getDistanceEstimate() override;

    void optimize() override {};

    int size() override;

private:
    double areaRadius;
    Ellipse area, region;

    vector<LineSegment> history;
    vector<DistanceEstimate> distanceEstimate;

    double withinSpokeLength = 0;
    double spokeArea = 0;
    double distance = 0;
    int spokes = 0;
};


#endif //PLUME_EXPERIMENTS_JOHNSPOKESTRATEGY_H
