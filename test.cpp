#include <iostream>
#include <random>
#include <queue>
#include "Point.h"
#include "SearchStrategyFactory.h"

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

    TSP::optimize(points);

    cout << "Optimized: " << endl;

//    for(Point* point : points) {
//        cout << point->getX() << "," << point->getY() << endl;
//    }

    cout << "Distance " << distance(points) << endl;
}

int main() {

    testKDTree();
    testTSP();

    return 0;
}