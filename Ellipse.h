#ifndef PLUME_EXPERIMENTS_ELLIPSE_H
#define PLUME_EXPERIMENTS_ELLIPSE_H

#include "Point.h"
#include "LineSegment.h"
#include "Plume.h"

class Ellipse: public Plume {

public:
    Ellipse(const Point &center, double x_radius, double y_radius);

    bool inside(const Point &vector);

    double size();

    LineSegment segmentIntersections(LineSegment &segment);

    LineSegment intersections(Line &line);

    bool crosses(LineSegment &segment);

    bool crossesEdge(LineSegment &segment);

    Point getCross(LineSegment &segment) override;

    double edgeGradient(Point& point);

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

    void print(std::ostream &ostream) const override;

};

#endif //PLUME_EXPERIMENTS_ELLIPSE_H
