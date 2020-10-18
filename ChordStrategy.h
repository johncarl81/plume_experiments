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

    void execute() override;

    double getDistance() override;

    double getAreaEstimate() override;

private:
    Ellipse area, region;
    double areaRadius;

    LineSegment startEndCircle();
    LineSegment randomAngle();
    LineSegment midpoint();

    double distance = 0;
    double totalRegionChordLength = 0;
    int samples = 0;

    Point randomPoint();
};


#endif //PLUME_EXPERIMENTS_CHORDSTRATEGY_H
