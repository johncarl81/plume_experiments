#include <iostream>
#include <random>
#include <queue>
#include "Point.h"
#include "SearchStrategyFactory.h"

void randomSpoke(Ellipse ellipse, int i);

using namespace std;

void assertNearest(KDTree &tree, Point input, Point *nearest) {
    PointDistance output = tree.nearest(&input);
    bool pass = output.first == nearest;
    cout << "Passed: " << (pass ? "true" : "false") << endl;

    if(!pass) {
        cout << "Failed with output (" << output.first->getX() << "," << output.first->getY() << ") with distance: " << output.second << endl;
    }
}

void testKDTree() {
    KDTree tree;

    vector<Point*> points;

    points.push_back(new Point(0, 0));
    points.push_back(new Point(0, 1));
    points.push_back(new Point(0, 2));
    points.push_back(new Point(2, 3));
    points.push_back(new Point(-1, -1));

    for(auto point : points) {
        tree.insert(point);
    }

    for(auto & point : points) {
        assertNearest(tree, *point, point);
    }

    assertNearest(tree, Point(0, 0.6), points[1]);
    assertNearest(tree, Point(-0.9, -0.5), points[4]);

    tree.remove(points[1]);

    assertNearest(tree, Point(0, 1), points[2]);

    for(Point* point : points) {
        delete point;
    }
}

double distance(vector<LineSegment*> &edges) {
    double distance = 0;

    bool firstLoop = true;
    for(int i = 0; i < edges.size(); i++) {
        if(firstLoop) {
            firstLoop = false;
        } else {
            Point start = edges[i-1]->getStart();
            Point end = edges[i]->getEnd();
            distance += (start - end).length();
        }
        distance += edges[i]->length();
    }

    return distance;
}

double distance(vector<Point*> &points) {
    double distance = 0;

    bool firstLoop = true;
    for(int i = 0; i < points.size(); i++) {
        if(firstLoop) {
            firstLoop = false;
        } else {
            distance += ((*points[i]) - (*points[i-1])).length();
        }
    }

    return distance;
}

void testTSP() {

    vector<Point*> empty;

    TSP::optimize(empty);

    vector<Point*> one;
    one.push_back(new Point(0, 0));

    TSP::optimize(one);

    vector<Point*> points;

    points.push_back(new Point(12, 30));
    points.push_back(new Point(44, 15));
    points.push_back(new Point(32, 28));
    points.push_back(new Point(95, 80));
    points.push_back(new Point(0, 60));
    points.push_back(new Point(55, 70));
    points.push_back(new Point(15, 70));

    TSP::optimize(points);

    points.clear();

    int size = 262143;

    for(int i = 0; i < size; i++) {
        points.push_back(new Point(RandomUtil::uniform_random(0, 6), RandomUtil::uniform_random(0, 6)));
    }

//    for(Point* point : points) {
//        cout << point->getX() << "," << point->getY() << endl;
//    }


    cout << "Distance " << distance(points) << endl;

    time_t start = time(nullptr);
    TSP::optimize(points);
    cout << "Took: " << (time(nullptr) - start) << endl;

    cout << "Optimized: " << points.size() << endl;

//    for(Point* point : points) {
//        cout << point->getX() << "," << point->getY() << endl;
//    }

    cout << "Post distance " << distance(points) << endl;
}

LineSegment* buildLineSegment(Ellipse region, double m, double b) {
    Line line(m, b);
    LineSegment segment = region.intersections(line);
    return new LineSegment(segment);
}

LineSegment* randomMidpointRadius(Ellipse region, double radius) {
    double angle = RandomUtil::random_angle();
    double dist = radius *  RandomUtil::uniform_random();

    Point point = Point(cos(angle) * dist, sin(angle) * dist);

    Line line = Line::buildByPointAndAngle(point, angle + (M_PI_2));

    LineSegment segment = region.intersections(line);
    return new LineSegment(segment);
}

LineSegment* randomSpoke(int areaRadius) {
    double angle = RandomUtil::random_angle();

    Point end = Point(cos(angle) * areaRadius, sin(angle) * areaRadius);
    Point center = Point(0, 0);
    Line line = Line::buildByPointAndAngle(center, angle);

    return new LineSegment(line, center, end);
}

void testEdgeTSP() {
    vector<LineSegment *> segments;

    Ellipse region(Point(0, 0), 1, 1);

    segments.push_back(buildLineSegment(region, 0, 0));
    segments.push_back(buildLineSegment(region, 1, 0.5));
    segments.push_back(buildLineSegment(region, -1, 0.25));
    segments.push_back(buildLineSegment(region, 2, 0.25));

    cout << "Distance: " << distance(segments) << endl;

//    for(auto segment : segments) {
//        cout << segment->getStart().getX() << "," << segment->getStart().getY() << " -> " << segment->getEnd().getX() << "," << segment->getEnd().getY() << endl;
//    }

    vector<Point*> path = TSP::optimize(segments);

    cout << "Post Distance: " << distance(path) << " " << path.size() << endl;

//    for(Point* point : path) {
//        cout << point->getX() << "," << point->getY() << endl;
//    }
//
//    for(auto segment : segments) {
//        cout << segment->getStart().getX() << "," << segment->getStart().getY() << " -> " << segment->getEnd().getX() << "," << segment->getEnd().getY() << endl;
//    }

    segments.clear();

    int size = 1000;

    for(int i = 0; i < size; i++) {
        segments.push_back(randomMidpointRadius(region, 1));
    }

    cout << "Distance: " << distance(segments) << " " << segments.size() << endl;

    path = TSP::optimize(segments);

    cout << "Post Distance: " << distance(path) << " " << path.size() << endl;

    segments.clear();

    size = 1000;

    for(int i = 0; i < size; i++) {
        segments.push_back(randomSpoke(1));
    }

    cout << "Distance: " << distance(segments) << " " << segments.size() << endl;

    path = TSP::optimize(segments);

    cout << "Post Distance: " << distance(path) << " " << path.size() << endl;
}

void testInfrastructure() {
    Ellipse ellipse = Ellipse(Point(0, 0), 1, 1);

    Point crossStart = Point(0.5, 0);
    Point crossEnd = Point(1.5, 0);
    LineSegment crossingSegment = LineSegment(crossStart, crossEnd);

    Point notCrossStart = Point(1.5, 0);
    Point notCrossEnd = Point(2.5, 0);
    LineSegment notCrossingSegment = LineSegment(notCrossStart, notCrossEnd);

    // 1. Cross plume point given a line segment.
    if(ellipse.crossesEdge(crossingSegment)) {
        cout << "Crossing success: " << ellipse.getCross(crossingSegment) << endl;;
    } else {
        cout << "Should have crossed" << endl;;
    }

    if(ellipse.crossesEdge(notCrossingSegment)) {
        cout << "Should not have crossed" << endl;;
    } else {
        cout << "Not crossing success." << endl;
    }

    Point one = Point(0, 0);
    Point two = Point(1, 0);
    Point clockwise_three = Point(1, -1);
    Point counterclockwise_three = Point(1, 1);

    // 2. Clockwise/counterclockwise orientations given three points (cross product)
    cout << "Clockwise? " << (PointUtil::orientation(one, two, clockwise_three) == PointUtil::CLOCKWISE) << endl;
    cout << "Counterclockwise? " << (PointUtil::orientation(one, two, counterclockwise_three) == PointUtil::COUNTERCLOCKWISE) << endl;

    // 3. Point movement to a direction with a certain length. (input: angle, length)
    Point fortyfive = PointUtil::vector(M_PI_4, 1);
    cout << "45 degrees unitary: " << fortyfive << endl;
    Point negativefortyfive = PointUtil::vector(-M_PI_4, 1);
    cout << "-45 degrees unitary: " << negativefortyfive << endl;

    // 4. A vector and an orientation (clock/counter-clock), returns another point a distance of (variable d) perpendicular to the direction of this
    // vector and on the side of the orientation. We need this for the follower drone to move along the nearest drone.
    cout << "Perpendicular: " << PointUtil::perpendicular(one, two, 1, PointUtil::CLOCKWISE) << endl;
    cout << "Perpendicular: " << PointUtil::perpendicular(one, two, 1, PointUtil::COUNTERCLOCKWISE) << endl;

    Point diag = Point(1, 1);
    cout << "Perpendicular: " << PointUtil::perpendicular(one, diag, 1, PointUtil::CLOCKWISE) << endl;
    cout << "Perpendicular: " << PointUtil::perpendicular(one, diag, 1, PointUtil::COUNTERCLOCKWISE) << endl;

    // 8. Gradient class - returns the gradient given a point, either by differentiating, or by implicit function or, by bootstrapping.
    Point gradientIntersectionStart = Point(0, 0);
    Point gradientIntersectionEnd = Point(1, 1);
    LineSegment gradientIntersection = LineSegment(gradientIntersectionStart, gradientIntersectionEnd);
    Point cross = ellipse.getCross(gradientIntersection);
    cout << "Intersection " << cross << endl;
    cout << "Edge gradient: " << ellipse.edgeGradient(cross) << endl;

}

int main() {

    cout << "** KD Tree Tests **" << endl;
    testKDTree();
    cout << endl << "** TSP Tests **" << endl;
    testTSP();
    cout << endl << "** Edge TSP Tests **" << endl;
    testEdgeTSP();
    cout << endl << "** Sketch infrastructure tests **" << endl;
    testInfrastructure();

    return 0;
}
