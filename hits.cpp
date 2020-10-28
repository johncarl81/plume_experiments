#include <iostream>
#include <cmath>
#include <random>
#include "Point.h"
#include "Ellipse.h"
#include "SearchStrategyFactory.h"

using namespace std;

typedef pair<bool, double> HitLength;

HitLength simulate(double radiusA, double radiusS,
             SearchStrategy *(*strategyFactory)(double, const Ellipse &, const Ellipse &)) {
    int trials = 10000;

    Ellipse area(Point(0, 0), radiusS, radiusS);
    Ellipse plume(Point(RandomUtil::uniform_random() * (radiusS - radiusA),
                        RandomUtil::uniform_random() * (radiusS - radiusA)), radiusA, radiusA);

    SearchStrategy *strategy = strategyFactory(radiusS, area, plume);

    for (int t = 0; t < trials; t++) {
        strategy->execute();
    }

    double estimate = strategy->getAreaEstimate(trials);
    double actual = M_PI * radiusA * radiusA;
    double distance = strategy->getMaxDistance();

    delete strategy;
    HitLength hitLength;
    hitLength.second = distance;
    hitLength.first = abs(actual / estimate - 1) <= 0.05;

    return hitLength;
}

void countHits(const string &name, SearchStrategy *(*strategyFactory)(double, const Ellipse &, const Ellipse &)) {
    int cases = 100;
    int maxr = 10;

    int hits = 0;
    double distance = 0;
    for (int i = 1; i < cases; i++) {
        double r1, r2;
        r2 = RandomUtil::uniform_random() * maxr;
        r1 = RandomUtil::uniform_random() * r2;
        HitLength simulationHitLength = simulate(r1, r2, strategyFactory);
        if (simulationHitLength.first) {
            hits++;
            distance += simulationHitLength.second;
        }
    }
    cout << name << " Hits: " << hits << "/" << cases << "=" << (1.0 * hits / cases) << " average hit distance: "
         << (distance / hits) << endl;
}

int main() {
    countHits("Point by samples", SearchStrategyFactory::pointStrategy);
    countHits("Chords by length", SearchStrategyFactory::chordStrategy);
    countHits("Spokes by length", SearchStrategyFactory::spokeStrategy);
    countHits("Spokes by samples", SearchStrategyFactory::johnStrategy);

    return 0;
}