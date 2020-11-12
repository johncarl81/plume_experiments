#include <iostream>
#include <cmath>
#include <random>
#include "Point.h"
#include "Ellipse.h"
#include "SearchStrategyFactory.h"

using namespace std;

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

template <class T>
void coutArray(T data[], const int size) {
    bool first = true;
    cout << "[";
    for(int i = 0; i < size; i++) {
        if(first) {
            first = false;
        } else {
            cout << ", ";
        }
        cout << data[i];
    }
    cout << "]";
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

    int endingSizes[strategies.size()];
    for(int i = 0; i < strategies.size(); i++) {
        endingSizes[i] = strategies[i]->size();
    }


    cout << "            \"" << name << "\":" << endl;
    cout << "              {" << endl;
    cout << "                \"Convergence to " << thresholdWithin << "\": ";
    coutArray(trialsWithinResults, evaluations_size);
    cout << "," << endl;

    cout << "                \"means\": ";
    coutArray(areaMean, evaluations_size);
    cout << "," << endl;

    cout << "                \"variance\": ";
    coutArray(areaVariance, evaluations_size);
    cout << endl;

    cout << "                \"endingsizes\": ";
    coutArray(endingSizes, strategies.size());
    cout << endl;


    cout << "              }";

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
//    double evaluations[] = {0.5, 0.1, 0.05, 0.01, 0.001, 0.0001};
    int evaluations_size = 10;
    double evaluations[evaluations_size];
    for(int i = 1; i <= evaluations_size; i++) {
        evaluations[i-1] = pow(0.5, i);
    }
    double thresholdWithin = 0.9;
    cout << "        {" << endl;
    cout << "          \"Target size\":" << region.size() << "," << endl;
    cout << "          \"ellipse\": {\"center\": [" << region.getCenter().getX() << ", " << region.getCenter().getY() << "], \"rx\": " << region.getXRadius() << ", \"ry\": " << region.getYRadius() << "}, " << endl;
    cout << "          \"evaluations\": ";
    coutArray(evaluations, evaluations_size);
    cout << "," << endl;
    cout << "          \"techniques\": {" << endl;
    evaluate("Point by samples", SearchStrategyFactory::pointStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
    cout << "," << endl;
    evaluate("Chords by length", SearchStrategyFactory::chordStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
    cout << "," << endl;
//    evaluate("Spokes by length", SearchStrategyFactory::spokeStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
//    cout << "," << endl;
    evaluate("Spokes by samples", SearchStrategyFactory::johnStrategy, area, areaRadius, trials, limit, region, thresholdWithin, evaluations, evaluations_size);
    cout << endl;
    cout << "          }" << endl;
    cout << "        }";
}

int main() {
    time_t start = time(nullptr);

    cout.precision(10);

    double R = 3.0;
    int trials = 30;
    int limit = 10000000;

    Ellipse area = Ellipse(Point(0, 0), R, R);

    double ratios[] = {1, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3};

    cout << "{" << endl;

    cout << "  \"squash\":"  << endl;
    cout << "    {" << endl;
    bool first = true;
    for(double ratio : ratios) {
        if(first) {
            first = false;
        } else {
            cout << "," << endl;
        }
        cout << "      \"Ellipse: (0,0) (R,R/" << ratio << ")\": " << endl;
        Ellipse region = Ellipse(Point(0, 0), R, R / ratio);
        evaluate(area, R, trials, limit, region);
    }
    cout << endl;

    cout << "    }," << endl;

    double offsets[] = {0, 0.1, 0.2, 0.3, 0.4, 0.5};
    double radius_ratio = 1.2;

    cout << "  \"translate\":"  << endl;
    cout << "    {" << endl;
    first = true;
    for(double offset : offsets) {
        if(first) {
            first = false;
        } else {
            cout << "," << endl;
        }
        cout << "      \"Ellipse: (0," << offset << ") (R/" << radius_ratio << ",R/" << radius_ratio << ")\": " << endl;
        Ellipse region = Ellipse(Point(0, offset), R/radius_ratio, R/radius_ratio);
        evaluate(area, R, trials, limit, region);
    }
    cout << endl;
    cout << "    }," << endl;

    cout << "  \"Simulation took\": " << (time(nullptr) - start) << endl;

    cout << "}" << endl;



    return 0;
}