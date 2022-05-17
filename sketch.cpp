
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

void sketch(Point& start_a, Point& start_b, Plume& plume) {
    std::cout << plume;
    Point prev_a = Point(start_a.getX(), start_a.getY());
    Point prev_b = Point(start_b.getX(), start_b.getY());
    Point travel = Point(TRAVEL_DISTANCE, 0);
    Point curr_a = prev_a + travel;
    Point curr_b = prev_b + travel;
    int encounteredPlume = 0;
    Point gradient_start = curr_a;
    Point gradient_end = curr_b;
    std::cout << "Pen r" << std::endl;
    for(int i = 0; i < 40; i++) {
        LineSegment path_a = LineSegment(prev_a, curr_a);
        LineSegment path_b = LineSegment(prev_b, curr_b);

        if (encounteredPlume < 2) {
            std::cout << "Testing enountered" << std::endl;
            if(plume.crosses(path_a)) {
                gradient_end = gradient_start;
                gradient_start = plume.getCross(path_a);
                std::cout << "Loading a " << gradient_start << std::endl;
                encounteredPlume++;
            }
            if(plume.crosses(path_b)) {
                gradient_end = gradient_start;
                gradient_start = plume.getCross(path_b);
                std::cout << "Loading b " << gradient_start << std::endl;
                encounteredPlume++;
            }
        } else {

            Line gradient = Line::buildByPoints(gradient_start, gradient_end);

            double normalization = TRAVEL_DISTANCE * sqrt(1 / (pow(gradient.getM(), 2) + 1));
            double rise = normalization;
            double run = gradient.getM() * normalization;

            std::cout << "M: " << gradient.getM() << " Rise: " << rise << " Run: " << run << std::endl;

            travel = Point(rise, run);
        }

        std::cout << "Pen r" << std::endl;
        std::cout << LineSegment(prev_a, curr_a);
        std::cout << "Pen b" << std::endl;
        std::cout << LineSegment(prev_b, curr_b);

        prev_a = curr_a;
        prev_b = curr_b;

        curr_a = prev_a + travel;
        curr_b = prev_b + travel;
    }
}

int main() {
    double R = 1.0;
    Ellipse plume1 = Ellipse(Point(0, 0.4), R, R/2);
    Ellipse plume2 = Ellipse(Point(0, -0.4), R, R/2);
    Point start_a = Point(-1.31, 0.11);
    Point start_b = Point(-1.31, 0.01);

    CompositePlume plume = CompositePlume(&plume1, &plume2);

    sketch(start_a, start_b, plume);
}