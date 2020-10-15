
#ifndef PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
#define PLUME_EXPERIMENTS_SEARCHSTRATEGY_H


class SearchStrategy {

public:
    virtual void execute() = 0;

    virtual double getDistance() = 0;

    void executeUntil(double distance);

    virtual double getAreaEstimate() = 0;
};


#endif //PLUME_EXPERIMENTS_SEARCHSTRATEGY_H
