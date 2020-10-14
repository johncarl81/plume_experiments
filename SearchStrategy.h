
#ifndef PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
#define PLUME_EXPERIMENTS_SEARCHSTRATEGY_H


class SearchStrategy {

public:

    virtual void executeUntil(long distance) = 0;

    virtual double getAreaEstimate() = 0;
};


#endif //PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
