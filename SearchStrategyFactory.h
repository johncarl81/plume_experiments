//
// Created by john on 10/15/20.
//

#ifndef PLUME_EXPERIMENTS_SEARCHSTRATEGYFACTORY_H
#define PLUME_EXPERIMENTS_SEARCHSTRATEGYFACTORY_H

#include "SearchStrategy.h"
#include "Ellipse.h"
#include "PointStrategy.h"
#include "ChordStrategy.h"
#include "SpokeStrategy.h"
#include "JohnSpokeStrategy.h"

class SearchStrategyFactory {

public:

    static SearchStrategy *pointStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) {
        return new PointStrategy(areaRadius, area, region);
    }

    static SearchStrategy *chordStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) {
        return new ChordStrategy(areaRadius, area, region);
    }

    static SearchStrategy *spokeStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) {
        return new SpokeStrategy(areaRadius, area, region);
    }

    static SearchStrategy *johnStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) {
        return new JohnSpokeStrategy(areaRadius, area, region);
    }
};

#endif //PLUME_EXPERIMENTS_SEARCHSTRATEGYFACTORY_H
