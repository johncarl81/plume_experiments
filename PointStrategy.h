#ifndef PLUME_EXPERIMENTS_POINTSTRATEGY_H
#define PLUME_EXPERIMENTS_POINTSTRATEGY_H

#include <random>

#include "Ellipse.h"
#include "SearchStrategy.h"
#include "RandomUtil.h"

using namespace std;

class PointStrategy: public SearchStrategy  {

public:
    PointStrategy(double areaRadius, const Ellipse &area, const Ellipse &region);

    double getAreaEstimate() override;

    void executeUntil(long distance) override;

private:
    double areaRadius;
    Ellipse area;
    Ellipse region;

    long hits = 0;
    long samples = 0;
    Point previous = Point(0, 0);
    double length = 0;
    bool first = true;
};


#endif //PLUME_EXPERIMENTS_POINTSTRATEGY_H
