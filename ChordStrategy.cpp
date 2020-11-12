#include "ChordStrategy.h"

ChordStrategy::ChordStrategy(double areaRadius, const Ellipse &area, const Ellipse &region) : area(area),
                                                                                              region(region),
                                                                                              areaRadius(areaRadius),
                                                                                              previous(Point(0, 0)){}

void ChordStrategy::execute() {
    LineSegment chord = randomMidpointRadius();
    Line chordLine = chord.getLine();
    LineSegment regionChord = region.intersections(chordLine);

    distance += chord.length();
    if (!first) {
        distance += (chord.getStart() - previous).length();
    } else {
        first = false;
    }
    previous = chord.getEnd();

    totalRegionChordLength += regionChord.length();

    samples++;

    history.push_back(new LineSegment(chord));
    distanceEstimate.emplace_back(distance, 2 * areaRadius * (totalRegionChordLength / samples));
}

vector<DistanceEstimate>* ChordStrategy::getDistanceEstimate() {
    return &distanceEstimate;
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

LineSegment ChordStrategy::randomMidpointRadius() {
    double angle = RandomUtil::random_angle();
    double dist = areaRadius *  RandomUtil::uniform_random();

    Point point = Point(cos(angle) * dist, sin(angle) * dist);

    Line line = Line::buildByPointAndAngle(point, angle + (M_PI_2));

    return region.intersections(line);
}

Point ChordStrategy::randomPoint() {
    return Point(RandomUtil::uniform_random(-areaRadius, areaRadius),
            RandomUtil::uniform_random(-areaRadius, areaRadius));
}

double ChordStrategy::getMaxDistance() {
    return distance;
}

int ChordStrategy::size() {
    return history.size();
}

void ChordStrategy::optimize() {
    TSP::optimize(history);

    samples = 0;
    totalRegionChordLength = 0;
    distance = 0;
    first = true;
    for(int i = 0; i < history.size(); i++) {
        LineSegment* chord = history[i];
        Line chordLine = chord->getLine();
        LineSegment regionChord = region.intersections(chordLine);

        distance += chord->length();
        if (!first) {
            distance += (chord->getStart() - previous).length();
        } else {
            first = false;
        }
        previous = chord->getEnd();
        totalRegionChordLength += regionChord.length();

        samples++;

        distanceEstimate[i].distance = distance;
        distanceEstimate[i].estimate = 2 * areaRadius * (totalRegionChordLength / samples);
    }
}

ChordStrategy::~ChordStrategy() {

    for(LineSegment* edge : history) {
        delete edge;
    }
}

