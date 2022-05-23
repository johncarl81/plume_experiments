
#include "Ellipse.h"
#include "CompositePlume.h"

using namespace std;

const double epsilon = 0.0001;
const double inf = 1e-20;

double angleturned;
double disttraverse;

void CrossPlume (Drone &A, Drone &B, double alpha)
{
    Point prev = A.last ;
    Point curr = A.position ;
    Point next = move (curr);
    
    while (cross (prev, curr, next) == CONVEX && !cross_plume ())
    {
        prev = curr;
        curr = next;
        MoveDrone (A, alpha, epsilon * epsilon);
        
        if (alpha > 0)
            alpha += epsilon;
        else
            alpha -= epsilon;
        
        Sync (A,B,alpha);
        next = move (curr);
    }
    
    if (cross_plume () && cross (prev, curr, next) == CONVEX)
        MoveDrone (A, alpha, epsilon * epsilon);
    else{
        Line L (last, curr);
        next = straight_move (L, curr);
        while (!cross_plume())
        {
            next = straight_move (L, next);
            A.nabla = L.nabla;
            MoveDrone (A, 0, epsilon * epsilon);
        }
    }
    
    return;
}

void sketch (Drone A, Drone B, Point initial)
{
    int iter = 0;
    int TOKEN = 0;
    angleturned = 0;
    disttraverse = 0;
    
    do{
        while (TOKEN == A.inside + B.inside)
        {
            MoveDrone (A, alpha, epsilon);
            MoveDrone (B, alpha, epsilon);
        }
        if (A.inside + B.inside != 1)
        {
            if (A.inside + B.inside == 0)
            {
                B.nabla = A.nabla;
                CrossPlume (A, B, epsilon);
                alpha = epsilon;
            }
            else
            {
                A.nabla = B.nabla;
                CrossPlume (B, A, -epsilon);
                alpha = -epsilon;
            }
            iter ++;
        }
        TOKEN = A.inside + B.inside;
    }while (iter < 100 || (dist (A, initial) > inf && dist (B, initial) > inf ));
}

int main() {
    double R = 1.0;
    Ellipse plume1 = Ellipse(Point(0, 0.4), R, R/2);
    Ellipse plume2 = Ellipse(Point(0, -0.4), R, R/2);
    Point start_a = Point(-1.31, 0.11);
    Point start_b = Point(-1.31, 0.01);

    CompositePlume plume = CompositePlume(&plume1, &plume2);

    sketch(start_a, start_b, plume);
    
    return 0;
}
