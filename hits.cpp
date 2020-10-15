#include <iostream>
#include <cmath>
#include <random>
#include "Point.h"
#include "Ellipse.h"
#include "SearchStrategyFactory.h"

using namespace std;

int simulate(double radiusA, double radiusS,
             SearchStrategy *(*strategyFactory)(double, const Ellipse &, const Ellipse &)) {
    int trials = 10000;

    Ellipse area(Point(0, 0), radiusS, radiusS);
    Ellipse plume(Point(RandomUtil::uniform_random() * (radiusS - radiusA),
                        RandomUtil::uniform_random() * (radiusS - radiusA)), radiusA, radiusA);

    SearchStrategy *strategy = strategyFactory(radiusS, area, plume);

    for (int t = 0; t < trials; t++) {
        strategy->execute();
    }

    double estimate = strategy->getAreaEstimate();
    double actual = M_PI * radiusA * radiusA;

    delete strategy;

    if (abs(actual / estimate - 1) > 0.05) {
        return 0;
    }
    return 1;
}

void countHits(const string &name, SearchStrategy *(*strategyFactory)(double, const Ellipse &, const Ellipse &)) {
    int cases = 100;
    int maxr = 10000;

    int hits = 0;
    for (int i = 1; i < cases; i++) {
        double r1, r2;
        r2 = RandomUtil::uniform_random() * maxr;
        r1 = RandomUtil::uniform_random() * r2;
        hits += simulate(r1, r2, strategyFactory);
    }
    cout << name << " Hits: " << hits << "/" << cases << "=" << (1.0 * hits / cases) << endl;
}

int main() {
    countHits("Point", SearchStrategyFactory::pointStrategy);
    countHits("Chord", SearchStrategyFactory::chordStrategy);
    countHits("Spoke", SearchStrategyFactory::spokeStrategy);
    countHits("John", SearchStrategyFactory::johnStrategy);

    return 0;
}