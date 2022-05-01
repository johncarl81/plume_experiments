#ifndef PLUME_EXPERIMENTS_PLUME_H
#define PLUME_EXPERIMENTS_PLUME_H

#include "LineSegment.h"

class Plume {

public:
    virtual bool crosses(LineSegment &segment) = 0;

    virtual bool inside(const Point &point) = 0;

    friend std::ostream& operator<<(std::ostream &strm, const Plume &plume) {
        plume.print(strm);
        return strm;
    }

    virtual void print( std::ostream& ) const =0;
};

#endif //PLUME_EXPERIMENTS_PLUME_H
