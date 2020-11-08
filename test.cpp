#include <iostream>
#include <random>
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

void testTSP() {

    vector<Point> points;

    points.push_back(Point(12, 30));
    points.push_back(Point(44, 15));
    points.push_back(Point(32, 28));
    points.push_back(Point(95, 80));
    points.push_back(Point(0, 60));
    points.push_back(Point(55, 70));
    points.push_back(Point(15, 70));

    TSP::minimize(points);
}

int main() {
    testKDTree();
    testTSP();

    return 0;
}