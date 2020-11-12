#ifndef PLUME_EXPERIMENTS_ELLIPSE_H
#define PLUME_EXPERIMENTS_ELLIPSE_H

#include "Point.h"
#include "LineSegment.h"

class Ellipse {

public:
    Ellipse(const Point &center, double x_radius, double y_radius);

    bool inside(Point &vector);

    double size();

    LineSegment segmentIntersections(LineSegment &segment);

    LineSegment intersections(Line &line);

    Point getCenter() {
        return center;
    }

    double getXRadius() {
        return radius_x;
    }

    double getYRadius() {
        return radius_y;
    }

private:
    Point center;
    double radius_x, radius_y;
    double _size = M_PI * radius_x * radius_y;

};

#endif //PLUME_EXPERIMENTS_ELLIPSE_H
