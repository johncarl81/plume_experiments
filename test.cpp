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

int main() {

    cout << "** KD Tree Tests **" << endl;
    testKDTree();
    cout << endl << "** TSP Tests **" << endl;
    testTSP();
    cout << endl << "** Edge TSP Tests **" << endl;
    testEdgeTSP();

    return 0;
}
