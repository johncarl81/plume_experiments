
#ifndef PLUME_EXPERIMENTS_POINT_H
#define PLUME_EXPERIMENTS_POINT_H

#include <cmath>

class Point {
public:
    Point(double x, double y);

    Point operator+ (Point& first);

    Point operator- (Point& first);

    double length();

    double getX() const;

    double getY() const;

private:
    double x,y;

};

#endif //PLUME_EXPERIMENTS_POINT_H
