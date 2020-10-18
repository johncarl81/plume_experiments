#include <iostream>
#include <cmath>
#include <random>
#include "Point.h"
#include "Ellipse.h"
#include "SearchStrategyFactory.h"


using namespace std;

double averageDistance(vector<SearchStrategy *> strategies) {
    double distance_sum = 0;

    for (SearchStrategy *strategy : strategies) {
        distance_sum += strategy->getDistance();
    }

    return distance_sum / strategies.size();
}

void evaluateConvergence(const string name, vector<SearchStrategy *> strategies, const int limit, const int trials, const double regionSize, const double thresholdWithin, const double trialsWithin[], const int evaluations_size) {
    double trialsWithinResults[evaluations_size];
    for(int e = 0; e < evaluations_size; e++) {
        trialsWithinResults[e] = -1;
    }
    bool trialsPassed[evaluations_size];
    for(int e = 0; e < evaluations_size; e++) {
        trialsPassed[e] = false;
    }

    bool finished = false;
    for(int i = 0; i < limit && !finished; i += 1) {
        int trialsWithinCount[evaluations_size];
        for(int e = 0; e < evaluations_size; e++) {
            trialsWithinCount[e] = 0;
        }
        for (SearchStrategy* strategy : strategies) {
            strategy->executeUntil(i);
            double estimate = strategy->getAreaEstimate();

//            cout << estimate << " vs " << regionSize << endl;

            double relativeDifference = abs(estimate - regionSize);
            for(int e = 0; e < evaluations_size; e++) {
                if(!trialsPassed[e]) {
                    if (relativeDifference < regionSize * trialsWithin[e]) {
                        trialsWithinCount[e]++;
                    }
                }
            }
        }
//        cout << i << " " << name << ": ";

        finished = true;
        for(int e = 0; e < evaluations_size; e++) {
//            cout << trialsWithin[e] << " " << (trialsPassed[e] ? trialsWithinResults[e] : 1.0 * trialsWithinCount[e] / trials) << " ";
            if(!trialsPassed[e] && trialsWithinCount[e] >= thresholdWithin * trials) {
                trialsWithinResults[e] = averageDistance(strategies);
                trialsPassed[e] = true;
            }

            finished &= trialsPassed[e];
        }
//        cout << endl;

    }

    cout << name << ":\t@" << thresholdWithin << ": ";
    for(int e = 0; e < evaluations_size; e++) {
        cout << trialsWithin[e] << ": " << trialsWithinResults[e] << ",\t";
    }
    cout << endl;

    double estimateSum = 0;

    for (SearchStrategy* strategy : strategies) {
        estimateSum += strategy->getAreaEstimate();
    }
    cout << "Average: " << (estimateSum / strategies.size()) << endl;


}

void evaluate(const string name, SearchStrategy *(*strategyFactory)(double, const Ellipse &, const Ellipse &), Ellipse area, double radius, int trials, int limit, Ellipse region, double thresholdWithin, double trialsWithin[5], int evaluations_size) {
    vector<SearchStrategy*> strategies;
    strategies.reserve(trials);
    for(int t = 0; t < trials; t++) {
        strategies.push_back(strategyFactory(radius, area, region));
    }

    evaluateConvergence(name, strategies, limit, trials, region.size(), thresholdWithin, trialsWithin,
                        evaluations_size);

    for (SearchStrategy* strategy : strategies) {
        delete strategy;
    }
}

void evaluate(Ellipse area, double areaRadius, int trials, int limit, Ellipse region) {

    double evaluations[] = {0.5, 0.1, 0.05, 0.01, 0.001};
    int evaluations_size = 5;
    cout << "Target size " << region.size() << endl;

    evaluate("Point by samples", SearchStrategyFactory::pointStrategy, area, areaRadius, trials, limit, region, 0.9, evaluations, evaluations_size);
    evaluate("Chords by length", SearchStrategyFactory::chordStrategy, area, areaRadius, trials, limit, region, 0.9, evaluations, evaluations_size);
    evaluate("Spokes by length", SearchStrategyFactory::spokeStrategy, area, areaRadius, trials, limit, region, 0.9, evaluations, evaluations_size);
    evaluate("Spokes by samples", SearchStrategyFactory::johnStrategy, area, areaRadius, trials, limit, region, 0.9, evaluations, evaluations_size);
}

int main() {
    cout << "Running Plume Area Simulation Convergence..." << endl;
    time_t start = time(nullptr);

    double R = 8.0;
    int trials = 100;
    int limit = 10000000;

    Ellipse area = Ellipse(Point(0, 0), R, R);
    Ellipse region = Ellipse(Point(0, 0), R, R / 1.2);
    evaluate(area, R, trials, limit, region);

    cout << "Simulation took: " << (time(nullptr) - start) << "s" << endl;

    return 0;
}