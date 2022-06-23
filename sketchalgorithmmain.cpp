#include <cmath>
#include <vector>
#include <cstdio>
#include <random>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

const int ITER = 10;
const int CROSSBOUND = 20;
const double R = 0.5;
const double PI = 3.14159;
const double DIST = 2;

double epsilon, INF;

vector<double> areas, lengths, angles, eps;

class Point {
public:

    Point (){}
    Point(double x, double y);

    Point operator+ (Point& first);

    Point operator- (Point& first);

    Point operator+= (Point& first);

    Point operator-= (Point& first);

    Point operator-();

    double length();

    double getX() const;

    double getY() const;

    bool operator< (const Point& other) const {
        return other.x < this->x || (other.x == this->x && other.y < this->y);
    }

    double x,y;

};

class Line {

public:
    Line(double m, double b);

    static Line buildByPoints(Point& start, Point& end);

    static Line buildByPointAndAngle(Point& start, double angle);

    double getM() const;

    double getB() const;

private:
    double m, b;
};


class PointUtil {

public:
    static double orientation(Point& one, Point& two, Point& three);

    static Point vector(double angle, double length);

    static Point perpendicular(Point& one, Point& two, double length, int orientation);

    static const int CLOCKWISE = 1;
    static const int COUNTERCLOCKWISE = -1;
};


class LineSegment {

public:
    LineSegment(Point &start, Point &end);

    LineSegment(const Line &line, const Point &start, const Point &end);

    LineSegment(const LineSegment& copySegment);

    ~LineSegment();

    double length();

    Line getLine();

    Point getStart();

    Point* getStartPtr();

    Point getEnd();

    Point* getEndPtr();

private:
    Line line;
    Point *start, *end;
};

class Ellipse {

public:
    Ellipse (){}
    Ellipse(Point centergiven, double x_radius, double y_radius)
    {
        center = centergiven;
        radius_x = x_radius;
        radius_y = y_radius;
        _size = M_PI * radius_x * radius_y;
    }

    bool inside(const Point &vector);

    double size();

    LineSegment segmentIntersections(LineSegment &segment);

    LineSegment intersections(Line &line);

    bool crosses(LineSegment &segment);

    bool crossesEdge(LineSegment &segment);

    Point getCross(LineSegment &segment) ;

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
    double _size;
};


double changeGradient (double angleA, double angleB)
{
    return abs (angleA - angleB); //check this again for bug.
}

void reverse (double &gradient)
{
    if (gradient < 0)
        gradient = gradient + PI;
    else if (gradient > 0)
        gradient = gradient - PI;
    else
        gradient = 0 ;
    return ;
}

int checkin (Point P){
    double A = R;
    double B = R/2;
    return (((P.x * P.x) / (A * A) + (P.y * P.y) / (B*B))  <= 1);
}


class Drone {
    public :
    Point position;
    Point last;
    double nabla ; //just the gradient angle, not the slope
    int inside, numCross ;
    bool droneIn;
    double angleTurned;
    double distTraversed;
    vector<Point> polytope;
    
    Drone (){}
    Drone (Point P1, Point P2, int in, double nab, bool flag)
    {
        position = P1;
        last = P2;
        inside = in;
        nabla = nab;
        droneIn = flag;
        angleTurned = 0;
        distTraversed = 0;
        numCross = 0;
     //   polytope.push_back (position);
    }
    
    bool MoveDrone (double alpha, double dist, Ellipse &plume)
    {
        Point motion;
        Point nextPosition;
        motion = PointUtil::vector (nabla + alpha, dist);
        nextPosition = position + motion;
        LineSegment dronemotion = LineSegment (position, nextPosition);
        
        distTraversed += dist;
        
        if (abs(nextPosition.x) > 3 || abs(nextPosition.y) > 3)
            cout << "position exception! "<<endl;
        
       // bool cross = plume.crossesEdge (dronemotion);
        int cross = checkin (position) ^ checkin (nextPosition);
        if (cross)
        {
            ++numCross;
            inside = inside ^ 1;
           // if (abs(alpha) > epsilon)
             //   cout<<"Testing alpha " << inside<<endl;
            Point crossingPoint = plume.getCross (dronemotion);
            double gradient = plume.edgeGradient (crossingPoint);
            Point checkPoint = PointUtil::vector (gradient, dist);
            checkPoint = crossingPoint + checkPoint;
            
            int orient ;
            if (inside)
                orient = PointUtil::CLOCKWISE;
            else
                orient = PointUtil::COUNTERCLOCKWISE;
           // else
             //   orient = (inside) ? (PointUtil::COUNTERCLOCKWISE) : (PointUtil::CLOCKWISE);
            
            if (PointUtil::orientation (position, crossingPoint, checkPoint) != orient)
                reverse(gradient);
            angleTurned += changeGradient (nabla + alpha, gradient);
            nabla = gradient;
            if (numCross == 1)
            cout << crossingPoint.x <<  " crossed here "<< crossingPoint.y <<  " " <<droneIn<<" inside: "<<inside<<" "<< gradient<< endl;
        }
        last = position;
        position = nextPosition;
        if (polytope.size())
            polytope.push_back (position);
        if (numCross == 1 && cross)
            polytope.push_back (plume.getCross(dronemotion));
        
        if (polytope.size() > 1)
        {
            Point currtoinit = polytope.back() - polytope[0];
            return (currtoinit.length() < INF) && (numCross > CROSSBOUND);
        }
        else
            return false ;
    }
};

Point::Point(double x, double y) : x(x), y(y) {}

Point Point::operator+(Point &first) {
    return Point(x + first.x, y + first.y);
}

Point Point::operator+=(Point &first){
    this->x += first.x;
    this->y += first.y;
    return *this;
}

Point Point::operator-(Point &first) {
    return Point(x - first.x, y - first.y);
}

Point Point::operator-=(Point &first){
    this->x -= first.x;
    this->y -= first.y;
    return *this;
}

Point Point::operator-() {
    return Point(-this->x, -this->y);
}

double Point::length() {
    return sqrt(x * x + y * y);
}

double Point::getX() const {
    return x;
}

double Point::getY() const {
    return y;
}

double PointUtil::orientation(Point& one, Point& two, Point& three) {
    double k=(two.getY() - one.getY())*(three.getX() - two.getX())-(two.getX() - one.getX()) * (three.getY() - two.getY());

    if(k>0) {
        return CLOCKWISE;
    } else {

        return COUNTERCLOCKWISE;
    }
}

Point PointUtil::vector(double angle, double length) {
    return Point(length * cos(angle), length * sin(angle));
}

Point PointUtil::perpendicular(Point &one, Point &two, double length, int orientation) {
    double delta_x = two.getX() - one.getX();
    double delta_y = two.getY() - one.getY();
    double angle = atan2(delta_y, delta_x);
    return vector(angle + (orientation * M_PI_2), length);
}


Line::Line(double m, double b) : m(m), b(b) {}

Line Line::buildByPoints(Point &start, Point &end) {
    double m = (end.getY() - start.getY()) / (end.getX() - start.getX() + 1e-9); //divide by zero case solved by 1e-9
    double b = start.getY() - (m * start.getX());

    return Line(m, b);
}

Line Line::buildByPointAndAngle(Point &start, double angle) {
    double m = tan(angle);
    double b = start.getY() - (m * start.getX());

    return Line(m, b);
}

double Line::getM() const {
    return m;
}

double Line::getB() const {
    return b;
}

LineSegment::LineSegment(Point &start, Point &end) : line(Line::buildByPoints(start, end)), start(new Point(start.getX(), start.getY())), end(new Point(end.getX(), end.getY())) {}

LineSegment::LineSegment(const Line &line, const Point &start, const Point &end) : line(line), start(new Point(start.getX(), start.getY())), end(new Point(end.getX(), end.getY())) {}

LineSegment::LineSegment(const LineSegment &copySegment): line(copySegment.line), start(new Point(copySegment.start->getX(), copySegment.start->getY())), end(new Point(copySegment.end->getX(), copySegment.end->getY())) {}

double LineSegment::length() {
    Point vector = (*end - *start);
    return vector.length();
}

Line LineSegment::getLine() {
    return line;
}

Point LineSegment::getStart() {
    return *start;
}

Point* LineSegment::getStartPtr() {
    return start;
}

Point LineSegment::getEnd() {
    return *end;
}

Point* LineSegment::getEndPtr() {
    return end;
}

LineSegment::~LineSegment() {
    delete start;
    delete end;
}

bool Ellipse::inside(const Point &vector) {
    return pow(((vector.getX() - center.getX()) / radius_x), 2) +
           pow(((vector.getY() - center.getY()) / radius_y), 2) <= 1;
}

double Ellipse::size() {
    return _size;
}

LineSegment Ellipse::segmentIntersections(LineSegment &segment) {
    Line line = segment.getLine();
    LineSegment intersectionSegment = intersections(line);

    Point start = intersectionSegment.getStart();
    Point end = intersectionSegment.getEnd();

    if(start.getX() > end.getX()) {
        Point tmp = start;
        start = end;
        end = tmp;
    }

    Point segmentStart = segment.getStart();
    Point segmentEnd = segment.getEnd();

    if(segmentStart.getX() > segmentEnd.getX()) {
        Point tmp = segmentStart;
        segmentStart = segmentEnd;
        segmentEnd = tmp;
    }

    if(segmentStart.getX() > start.getX()) {
        start = segmentStart;
    }
    if(segmentEnd.getX() < end.getX()) {
        end = segmentEnd;
    }
    if(start.getX() > end.getX()) {
        start = end;
    }

    return LineSegment(segment.getLine(), start, end);
}

LineSegment Ellipse::intersections(Line &line) {
    double rx = radius_x * radius_x;
    double ry = radius_y * radius_y;

    double a = (1 / rx) + (line.getM() * line.getM() / ry);
    double b = (2 * line.getB() * line.getM() / ry) - (2 * center.getX() / rx) - (2 * center.getY() * line.getM() / ry);
    double c = (line.getB() * line.getB() / ry) - (2 * line.getB() * center.getY() / ry) + (center.getX() * center.getX() / rx) + (center.getY() * center.getY() / ry) - 1;

    // Solution using Quadratic equation -b +- sqrt(b^2 - 4ac)/2a
    // where ax^2 + bx + c = 0
    double discriminant = pow(b,2) - (4 * a * c);

    if (discriminant > 0){
        double x1 = ((-b) + sqrt(discriminant)) / (2 * a);
        double x2 = ((-b) - sqrt(discriminant)) / (2 * a);

        double y1 = (line.getM() * x1) + line.getB();
        double y2 = (line.getM() * x2) + line.getB();

        return LineSegment(line, Point(x1, y1), Point(x2, y2));
    } else {
        return LineSegment(line, Point(0, 0), Point(0, 0));
    }
}

bool Ellipse::crosses(LineSegment &segment) {
    Line line = segment.getLine();

    double rx = radius_x * radius_x;
    double ry = radius_y * radius_y;

    double a = (1 / rx) + (line.getM() * line.getM() / ry);
    double b = (2 * line.getB() * line.getM() / ry) - (2 * center.getX() / rx) - (2 * center.getY() * line.getM() / ry);
    double c = (line.getB() * line.getB() / ry) - (2 * line.getB() * center.getY() / ry) + (center.getX() * center.getX() / rx) + (center.getY() * center.getY() / ry) - 1;

    // Solution using Quadratic equation -b +- sqrt(b^2 - 4ac)/2a
    // where ax^2 + bx + c = 0
    double discriminant = pow(b,2) - (4 * a * c);

    if (discriminant > 0){
        double x1 = ((-b) + sqrt(discriminant)) / (2 * a);
        double x2 = ((-b) - sqrt(discriminant)) / (2 * a);

        return (segment.getStart().getX() < x1 && segment.getStart().getX() > x2)
            || (segment.getEnd().getX() < x1 && segment.getEnd().getX() > x2);
    } else {
        return false;
    }
}


bool Ellipse::crossesEdge(LineSegment &segment) {
    Line line = segment.getLine();
    LineSegment intersectionSegment = intersections(line);

    Point start = intersectionSegment.getStart();
    Point end = intersectionSegment.getEnd();

    Point segmentStart = segment.getStart();
    Point segmentEnd = segment.getEnd();
    
    //check if discriminant is negative
    if (start.x == 0 && start.y == 0 && end.x == 0 && end.y == 0)
        return false;

    if(segmentStart.getX() > segmentEnd.getX()) {
        Point tmp = segmentStart;
        segmentStart = segmentEnd;
        segmentEnd = tmp;
    }

    return (start.getX() > segmentStart.getX() && start.getX() < segmentEnd.getX()) ||
            (end.getX() > segmentStart.getX() && end.getX() < segmentEnd.getX());
}


Point Ellipse::getCross(LineSegment &segment) {
    Line line = segment.getLine();
    LineSegment intersectionSegment = intersections(line);

    Point start = intersectionSegment.getStart();
    Point end = intersectionSegment.getEnd();

    Point segmentStart = segment.getStart();
    Point segmentEnd = segment.getEnd();
    
    
    //check if discriminant is negative
    if (start.x == 0 && start.y == 0 && end.x == 0 && end.y == 0){
        cout<<"Exception!!"<<endl;
        return Point (0,0);
    }

    if(segmentStart.getX() > segmentEnd.getX()) {
        Point tmp = segmentStart;
        segmentStart = segmentEnd;
        segmentEnd = tmp;
    }
//there might be multiple points of intersection, revise this part
    if (start.getX() > segmentStart.getX() && start.getX() < segmentEnd.getX()) {
        return start;
    }
    if (end.getX() > segmentStart.getX() && end.getX() < segmentEnd.getX()) {
        return end;
    }
    return Point(0, 0);
}

double Ellipse::edgeGradient(Point& point) {
    return atan2(-pow(radius_y, 2) * point.getX(), (pow(radius_x, 2) * point.getY()));
}

void Sync (Drone &A, Drone &B, double alpha)
{
    Point v;
    
    if (alpha > 0)
        v = PointUtil::vector (A.nabla + alpha - PI/2, DIST * epsilon);
    else
        v = PointUtil::vector (A.nabla + alpha + PI/2, DIST * epsilon);
    
    Point nextPosition = A.position + v;
    B.last = nextPosition;
    B.position = nextPosition;
    B.nabla = A.nabla;
    
    return ;
}

string check (Point P){
    double A = R;
    double B = R/2;
    return (((P.x * P.x) / (A * A) + (P.y * P.y) / (B*B))  <= 1) ? "Inside " : "Outside ";
}

bool CrossPlume (Drone &A, Drone &B, double alpha, Ellipse &plume)
{
    Point start_pos = A.last;
   // if (abs(start_pos.x) < 2 && abs(start_pos.y) < 2)
     //   cout <<"start pos ... "<< start_pos.x << " " << start_pos.y <<" "<<check(A.position)<< endl;

    int crossing = A.inside;
    bool orient = true ;
    double alphainitial = alpha;
    bool endHere = false;
    
    do{
        endHere = endHere || A.MoveDrone (alpha, epsilon * epsilon, plume);
        
        if (PointUtil::orientation(A.last, A.position, start_pos) == PointUtil::CLOCKWISE && alpha > 0)
            orient = false ;
        if (PointUtil::orientation(A.last, A.position, start_pos) == PointUtil::COUNTERCLOCKWISE && alpha < 0)
            orient = false ;
        Sync (A,B,alpha);
        if (alpha > 0)
            alpha += epsilon;
        else
            alpha -= epsilon;
        A.angleTurned += abs (alphainitial);
  //      cout << "testing cross plume ... "<< A.position.x << " " << A.position.y << endl;
   //     cout << "testing cross plume ... "<< B.position.x << " " << B.position.y << endl;

    }while (crossing == A.inside && orient);
        
    if (crossing == A.inside && !endHere){
        A.polytope.pop_back ();
        A.position = A.last;
        A.angleTurned -= abs (alphainitial);
        double dx = start_pos.getX () - A.position.getX();
        double dy = start_pos.getY () - A.position.getY();
        double gradient = atan2 (dy, dx);
       
    //    A.position = A.last;
     //   B.position = B.last;
        
        Point d1 = start_pos - A.position;
        Point motion;
        if (d1.length() > epsilon*epsilon)
            motion = PointUtil::vector (gradient, epsilon * epsilon);
        else
            motion = PointUtil::vector (gradient, d1.length());
        motion = A.position + motion;
        Point d2 = start_pos - motion;
     //   cout<<A.position.x<<" " <<A.position.y<<" "<<d1.length()<< " distances " << d2.length()<<endl;

        if (d1.length() < d2.length()){
            reverse (gradient);
        }
        A.angleTurned += changeGradient (A.nabla + alpha, gradient);
        A.nabla = gradient;
     //   B.nabla = gradient;
     //   cout << start_pos.x << " starting here " << start_pos.y << " "<<gradient<<" "<<check (start_pos) <<" "<<check(A.position)<<" "<<A.inside<< endl;
     //   cout << "testing cross plume2 ... "<< A.position.x << " " << A.position.y << endl;
     //   cout << "testing cross plume2 ... "<< A.last.x << " " << A.last.y << endl;

        while (crossing == A.inside && !endHere){
            d1 = start_pos - A.position;
            if (d1.length() > epsilon * epsilon)
            {
                endHere = endHere || A.MoveDrone (0, epsilon*epsilon, plume);
             //   B.MoveDrone (0, epsilon*epsilon, plume);
            }
            else{
                endHere = endHere || A.MoveDrone (0, d1.length(), plume);
            //    B.MoveDrone (0, d1.length()/2, plume);
            }
            
            if (abs(A.position.x) > 2 || abs(A.position.y) > 2)
                break ;
            
        //    Sync2 (A,B, alpha);
            //cout << "testing cross plume2 ... "<< A.position.x << " " << A.position.y << endl;
            //cout << "testing cross plume2 ... "<< B.position.x << " " << B.position.y << endl;
        }
      //  B.nabla = A.nabla;
        Sync (A,B,alphainitial);
    }
    
    return endHere;
}

double estimateArea (vector<Point> polygon)
{
    double area = 0 ;
    int n = polygon.size ();
    
    for (int i = 0;i < n; i ++)
        area += polygon[i].x * polygon [(i+1)%n].y - polygon[i].y* polygon[(i+1)%n].x;
    
    area /=2 ;
    
    return area ;
}

void sketch_algorithm ()
{
    
    cout << "Running Sketch Algorithm for Epsilon = " << epsilon << endl;
    
    Point start_a (0.499992,0.00141421);
    Point start_b (0.5,0);
    
    Ellipse plume (Point (0,0), R, R/2);
    
    Drone B (Point (0,0), Point (0,0), 1, 0, false);
    Drone A (Point (0,DIST * epsilon), Point (0, DIST * epsilon), 1, 0, true);
    
    int TOKEN = 2;
    
    double alpha = 0;
    Point lineA = A.position - start_a;
    Point lineB = B.position - start_b;
    
    bool loopEnd = false ;
    
    do{
        while (A.numCross + B.numCross == 0 || 1 == A.inside + B.inside)
        {
         //   cout << "testing TOKEN ... "<< A.position.x << " "<< A.position.y <<" 1"<<  endl;

            if (abs(A.position.x) > 2 || abs(A.position.y) > 2)
                break ;
            
            if (abs(B.position.x) > 2 || abs(B.position.y) > 2)
                break ;
            loopEnd = loopEnd || A.MoveDrone (alpha, epsilon, plume);
         //   Sync (A,B,alpha);
            loopEnd = loopEnd || B.MoveDrone (alpha, epsilon, plume);
            
        }
        if (A.inside + B.inside != 1)
        {
            if (A.inside + B.inside == 0)
            {
                alpha = epsilon;
                loopEnd = loopEnd || CrossPlume (A,B, alpha, plume);
                B.nabla = A.nabla;

 //               cout << "testing Sync A... "<< A.position.x << " "<<A.position.y <<" "<<alpha<<" "<<A.nabla<< endl;
 //               cout << "testing Sync B... "<< B.position.x << " "<<B.position.y <<" "<<alpha<<" "<<B.nabla<< endl;
            }
            else
            {
                alpha = -epsilon;
                loopEnd = loopEnd || CrossPlume (B,A, alpha, plume);
                A.nabla = B.nabla;

   //             cout << "testing Sync B... "<< B.position.x << " "<<B.position.y <<" "<<alpha<<" "<<B.nabla<< endl;
   //             cout << "testing Sync A... "<< A.position.x << " "<<A.position.y <<" "<<alpha<<" "<<A.nabla<< endl;
            }
        }
    //    TOKEN = A.inside + B.inside;
     //   lineA = A.polytope.back() - A.polytope[0];
     //   lineB = B.polytope.back() - B.polytope[0];
      //  cout << "position and nabla "<<A.position.x << " "<< A.position.y << " "<<A.nabla<< endl;
     //   if (A.polytope.back().x < 2 && A.polytope.back().y < 2 )
    //    cout << "position of polytope "<<A.polytope.back().x << " "<<A.polytope.back().y << endl;
     //   cout << lineA.length() << " distance from origin" << endl;
    }while (!loopEnd);
    
    cout <<"Initial crossing with A is " << A.polytope[0].x << " " << A.polytope[0].y << " " <<lineA.length()<< endl;
    cout << "angle turned by A is " << " " << A.angleTurned << endl;
    cout << "distance traversed by A is "<< " " << A.distTraversed << endl;
    cout << "area estimated by A is " << " " << estimateArea (A.polytope) << endl;
    areas.push_back (estimateArea (A.polytope));
    lengths.push_back (A.distTraversed);
    angles.push_back (A.angleTurned);
    cout << "actual area is  " << PI * R * R/2 << endl;
    
}

int main()
{
    int i = 0;
    for (epsilon = 0.001; i < 12 ; epsilon += 0.001){
        INF = 4 * epsilon;
        if (epsilon == 0.007) continue;
        sketch_algorithm ();
        eps.push_back (epsilon);
        ++i;
    }
    
    cout <<"areas ";
    for (int i = 0;i < 12; i ++)
        cout << areas[i] << " ";
    
    cout << endl;
    
    
    cout <<"lengths ";
    for (int i = 0;i < 12; i ++)
        cout << lengths[i] << " ";
    
    cout << endl;
    
    
    cout <<"angles ";
    for (int i = 0;i < 12; i ++)
        cout << angles[i] << " ";
    
    cout << endl;
    
    cout <<"epsilons ";
    for (int i = 0;i < 12; i ++)
        cout << eps[i] << " ";
    
    cout << endl;
    
    return 0;
}


/*
 
 EPSILON = 0.05
 Point start_a (0.479583,0.0707107);
 Point start_b (0.5,0);
 angle turned by A is  97.4447
 distance traversed by A is  10.305
 
 EPSILON = 0.01
 Point start_a (0.499199,0.0141421);
 Point start_b (0.5,0);
 
 angle turned by A is  184.588
 distance traversed by A is  10.1943
 
 EPSILON = 0.005
 Point start_a (0.4998,0.00707107);
 Point start_b (0.5,0);
 angle turned by A is  63.9424
 distance traversed by A is  2.91295
 
 EPSILON = 0.001
 Point start_a (0.499992,0.00141421);
 Point start_b (0.5,0);
 
 angle turned by A is  185.67
 distance traversed by A is  2.929

 */
