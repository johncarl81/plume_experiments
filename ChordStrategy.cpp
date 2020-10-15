#include "ChordStrategy.h"

ChordStrategy::ChordStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : area(area),
                                                                                              region(region),
                                                                                              areaRadius(areaRadius) {}

void ChordStrategy::execute() {
    LineSegment chord = startEndCircle();
    LineSegment regionChord = region.intersections(chord.getLine());

    distance += chord.length();
    totalRegionChordLength += regionChord.length();

    samples++;
}

double ChordStrategy::getAreaEstimate() {
    // This is strange, why do we need a constant here?
    return 0.843805869 * M_PI * areaRadius * (totalRegionChordLength / samples);
}

LineSegment ChordStrategy::startEndCircle() {
    double startAngle = RandomUtil::random_angle();
    double endAngle = RandomUtil::random_angle();

    Point start = Point(cos(startAngle) * areaRadius, sin(startAngle) * areaRadius);
    Point end = Point(cos(endAngle) * areaRadius, sin(endAngle) * areaRadius);

    Line line = Line::buildByPoints(start, end);

    return region.intersections(line);
}

LineSegment ChordStrategy::randomAngle() {
    double startAngle = RandomUtil::random_angle();
    double angle = RandomUtil::random_angle();

    Point start = Point(cos(startAngle) * areaRadius, sin(startAngle) * areaRadius);

    Line line = Line::buildByPointAndAngle(start, angle);

    return region.intersections(line);
}

LineSegment ChordStrategy::midpoint() {
    // Midpoint technique: https://en.wikipedia.org/wiki/Bertrand_paradox_(probability)
    Point midpoint = randomPoint();
    while(midpoint.length() > areaRadius) {
        midpoint = randomPoint();
    }

    double angle = RandomUtil::random_angle();

    Line line = Line::buildByPointAndAngle(midpoint, angle);

    return region.intersections(line);
}

Point ChordStrategy::randomPoint() {
    return Point(RandomUtil::uniform_random(-areaRadius, areaRadius),
            RandomUtil::uniform_random(-areaRadius, areaRadius));
}

double ChordStrategy::getDistance() {
    return distance;
}
