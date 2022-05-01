
#include "Ellipse.h"
#include "CompositePlume.h"

double TRAVEL_DISTANCE = 0.1;

Point left(Point& travel) {
    if(travel.getX() > 0) {
        return Point(0, -TRAVEL_DISTANCE);
    } else if(travel.getY() < 0) {
        return Point(-TRAVEL_DISTANCE, 0);
    } else if(travel.getX() < 0) {
        return Point(0, TRAVEL_DISTANCE);
    } else {
        return Point(TRAVEL_DISTANCE, 0);
    }
}

Point right(Point& travel) {
    return -left(travel);
}

void manhattan(Point& start, Plume& plume) {
    std::cout << plume;
    Point prev = Point(start.getX(), start.getY());
    Point travel = Point(TRAVEL_DISTANCE, 0);
    Point current = prev + travel;
    bool encounteredPlume = false;
    for(int i = 0; i < 400; i++) {
        LineSegment path = LineSegment(prev, current);
        if(!encounteredPlume && plume.crosses(path)) {
            encounteredPlume = true;
        }

        if(encounteredPlume) {
            if (plume.inside(current)) {
                // Turn Left
                travel = left(travel);
                current += travel;
            } else {
                // Outside of plume, turn Right
                travel = right(travel);
                current += travel;
            }
        } else {
            // Keep going straight until you encounter plume
            current += travel;
        }
        std::cout << LineSegment(prev, current);

        prev = current;
    }
}

int main() {
    double R = 1.0;
    Ellipse plume1 = Ellipse(Point(0, 0.4), R, R/2);
    Ellipse plume2 = Ellipse(Point(0, -0.4), R, R/2);
    Point start = Point(-1.31, 0.11);

    CompositePlume plume = CompositePlume(&plume1, &plume2);

    manhattan(start, plume);
}