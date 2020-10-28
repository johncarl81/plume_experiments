#include <iostream>
#include <cmath>
#include <random>
#include "Point.h"
#include "Ellipse.h"
#include "SearchStrategyFactory.h"

using namespace std;

double averageDistance(vector<SearchStrategy *> strategies, int distance) {
    double distance_sum = 0;
    for (SearchStrategy *strategy : strategies) {
        distance_sum += strategy->getMaxDistance();
    }
    return distance_sum / strategies.size();
}

double averageArea(vector<SearchStrategy *> strategies, int distance) {
    double distance_sum = 0;
    for (SearchStrategy *strategy : strategies) {
        distance_sum += strategy->getAreaEstimate(distance);
    }
    return distance_sum / strategies.size();
}

double areaVarience(vector<SearchStrategy *> strategies, int distance) {
    double mean = averageArea(strategies, distance);
    double squaredDifferenceSum = 0;
    for (SearchStrategy *strategy : strategies) {
        squaredDifferenceSum += pow(strategy->getAreaEstimate(distance) - mean, 2);
    }
    return squaredDifferenceSum / strategies.size();
}

void evaluateConvergence(const string name, vector<SearchStrategy *> strategies, const int limit, const int trials, const double regionSize, const double thresholdWithin, const double trialsWithin[], const int evaluations_size) {
    double trialsWithinResults[evaluations_size];
    double areaMean[evaluations_size];
    double areaVariance[evaluations_size];
    for(int e = 0; e < evaluations_size; e++) {
        trialsWithinResults[e] = -1;
        areaMean[e] = 0;
        areaVariance[e] = 0;
    }
    bool trialsPassed[evaluations_size];
    for(int e = 0; e < evaluations_size; e++) {
        trialsPassed[e] = false;
    }

    bool finished = false;
    for(int i = 0; i < limit && !finished; i++) {
        int trialsWithinCount[evaluations_size];
        for(int e = 0; e < evaluations_size; e++) {
            trialsWithinCount[e] = 0;
        }
        for (SearchStrategy* strategy : strategies) {
            strategy->executeUntil(i);
            double estimate = strategy->getAreaEstimate(i);

//            cout << estimate << " vs " << regionSize << endl;
            for(int e = 0; e < evaluations_size; e++) {
                if(!trialsPassed[e]) {
                    if (abs(regionSize / estimate - 1) <= trialsWithin[e]) {
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
                trialsWithinResults[e] = i;
                areaMean[e] = averageArea(strategies, i);
                areaVariance[e] = areaVarience(strategies, i);
                trialsPassed[e] = true;
            }

            finished &= trialsPassed[e];
        }
//        cout << endl;

    }
    cout << name << ":" << endl;
    cout << " Convergence to " << thresholdWithin << ":\t";
    for(int e = 0; e < evaluations_size; e++) {
        cout << trialsWithin[e] << ": " << trialsWithinResults[e] << ",\t";
    }
    cout << endl;

    cout << " means:  \t\t\t\t";
    for(int e = 0; e < evaluations_size; e++) {
        cout << trialsWithin[e] << ": " << areaMean[e] << ",\t";
    }
    cout << endl;

    cout << " variance:\t\t\t\t";
    for(int e = 0; e < evaluations_size; e++) {
        cout << trialsWithin[e] << ": " << areaVariance[e] << ",\t";
    }
    cout << endl;

//    cout << "Final mean: " << averageArea(strategies) << endl;
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

//    double evaluations[] = {0.5, 0.1, 0.05, 0.01, 0.001};
//    int evaluations_size = 5;
    double evaluations[] = {0.5, 0.1, 0.05, 0.01, 0.001, 0.0001};
    int evaluations_size = 4;
    double thresholdWithin = 0.9;
    cout << "Target size " << region.size() << endl;

    evaluate("Point by samples", SearchStrategyFactory::pointStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
    evaluate("Chords by length", SearchStrategyFactory::chordStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
    evaluate("Spokes by length", SearchStrategyFactory::spokeStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
    evaluate("Spokes by samples", SearchStrategyFactory::johnStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
}

int main() {
    cout << "Running Plume Area Simulation Convergence..." << endl;
    time_t start = time(nullptr);

    double R = 3.0;
    int trials = 30;
    int limit = 10000000;

    Ellipse area = Ellipse(Point(0, 0), R, R);

    double ratios[] = {1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2};

    for(double ratio : ratios) {
        cout << "Ellipse: (0,0) (R,R/" << ratio << ")" << endl;
        Ellipse region = Ellipse(Point(0, 0), R, R / ratio);
        evaluate(area, R, trials, limit, region);
    }

    double offsets[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5};
    double radius_ratio = 1.2;

    for(double offset : offsets) {
        cout << "Ellipse: (0," << offset << ") (R/" << radius_ratio << ",R/" << radius_ratio << ")" << endl;
        Ellipse region = Ellipse(Point(0, offset), R/radius_ratio, R/radius_ratio);
        evaluate(area, R, trials, limit, region);
    }

    cout << "Simulation took: " << (time(nullptr) - start) << "s" << endl;

    return 0;
}