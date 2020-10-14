#include "PointStrategy.h"

PointStrategy::PointStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : areaRadius(areaRadius),
                                                                                              area(area),
                                                                                              region(region) {}

void PointStrategy::executeUntil(long distance) {
    while(length < distance) {
        Point point = Point((RandomUtil::uniform_random() * 2 * areaRadius) - areaRadius,
                (RandomUtil::uniform_random() * 2 * areaRadius) - areaRadius);

        while (point.length() > areaRadius) {
            point = Point((RandomUtil::uniform_random() * 2 * areaRadius) - areaRadius,
                    (RandomUtil::uniform_random() * 2 * areaRadius) - areaRadius);
        }

        if (region.inside(point)) {
            hits++;
        }
        if (!first) {
            length += (previous - point).length();
        } else {
            first = false;
        }
        previous = point;
        samples = samples + 1;
    }
}

double PointStrategy::getAreaEstimate() {
    return 1.0 * area.size() * hits / samples;
}