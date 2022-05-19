/*
1. Cross plume point given a line segment.
2. Clockwise/counterclockwise orientations given three points (cross product)
3. Point movement to a direction with a certain length. (input: angle, length)
4. A vector and an orientation (clock/counter-clock), returns another point a distance of (variable d) perpendicular to the direction of this 
vector and on the side of the orientation. We need this for the follower drone to move along the nearest drone.
5. Cross Plume - need to change the invariant, depending on which drone crosses.
6. Angle turned data - I will write this.
7. Distance traversed - the above classes (#3 and #4) must return any distance traversed in context.
8. Gradient class - returns the gradient given a point, either by differentiating, or by implicit function or, by bootstrapping. 
*/

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

