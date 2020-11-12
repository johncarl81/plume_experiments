#ifndef PLUME_EXPERIMENTS_CHORDSTRATEGY_H
#define PLUME_EXPERIMENTS_CHORDSTRATEGY_H

#include <random>
#include "SearchStrategy.h"
#include "Ellipse.h"
#include "RandomUtil.h"

using namespace std;

class ChordStrategy: public SearchStrategy {
public:
    ChordStrategy(double areaRadius, const Ellipse &area, const Ellipse &region);

    ~ChordStrategy() override = default;

    void execute() override;

    double getMaxDistance() override;

    vector<DistanceEstimate>* getDistanceEstimate() override;

    void optimize() override {};

    int size() override;

private:
    Ellipse area, region;
    double areaRadius;

    vector<LineSegment> history;
    vector<DistanceEstimate> distanceEstimate;

    LineSegment startEndCircle();
    LineSegment randomAngle();
    LineSegment midpoint();
    LineSegment randomMidpointRadius();

    double distance = 0;
    double totalRegionChordLength = 0;
    int samples = 0;

    Point randomPoint();
};


#endif //PLUME_EXPERIMENTS_CHORDSTRATEGY_H
