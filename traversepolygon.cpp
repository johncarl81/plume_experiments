#include "point.h"
#include "line.h"

Point traverse_polygon (Point last, Point start)
{
    Point prev = last ;
    Point curr = start ;
    Point next = move (curr);
    
    while (cross (prev, curr, next) == CONVEX && !cross_plume ())
    {
        prev = curr;
        curr = next;
        next = move (curr);
    }
    
    if (cross_plume () && cross (prev, curr, next) == CONVEX)
        return next ;
    else{
        Line L (last, curr);
        next = straight_move (L, curr);
        while (!cross_plume())
            next = straight_move (L, next);
        return next ;
    }
    
    return NULL;
}

