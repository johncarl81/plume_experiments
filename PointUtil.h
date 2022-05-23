#ifndef PLUME_EXPERIMENTS_POINTUTIL_H
#define PLUME_EXPERIMENTS_POINTUTIL_H

#include <random>
#include <math.h>
#include "Point.h"

using namespace std;

class PointUtil {

public:
    static double orientation(Point& one, Point& two, Point& three);

    static Point vector(double angle, double length);

    static Point perpendicular(Point& one, Point& two, double length, int orientation);

    static const int CLOCKWISE = 1;
    static const int COUNTERCLOCKWISE = -1;
};


#endif //PLUME_EXPERIMENTS_POINTUTIL_H
