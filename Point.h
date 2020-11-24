
#ifndef PLUME_EXPERIMENTS_POINT_H
#define PLUME_EXPERIMENTS_POINT_H

#include <cmath>
#include <iostream>

class Point {
public:
    Point(double x, double y);

    Point operator+ (Point& first);

    Point operator- (Point& first);

    double length();

    double getX() const;

    double getY() const;

    bool operator< (const Point& other) const {
        return other.x < this->x || (other.x == this->x && other.y < this->y);
    }

    friend std::ostream& operator<<(std::ostream &strm, const Point &point) {
        return strm << "(" << point.x << "," << point.y << ")";
    }

private:
    double x,y;

};

#endif //PLUME_EXPERIMENTS_POINT_H
