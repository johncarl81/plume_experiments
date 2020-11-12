#ifndef PLUME_EXPERIMENTS_TSP_H
#define PLUME_EXPERIMENTS_TSP_H

#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <boost/heap/fibonacci_heap.hpp>
#include "Point.h"
#include "KDTree.h"
#include <iostream>


using namespace std;


class Edge {
public:
    Edge(Point *first, Point *second) : first(first), second(second) {
        distance = ((*first) - (*second)).length();
    };
    Edge(Point *first, PointDistance pointDistance) : first(first), second(pointDistance.first) {
        distance = pointDistance.second;
    };

    bool operator<(const Edge &first) const {
        return distance > first.distance;
    }

    bool operator>(const Edge &first) const {
        return distance < first.distance;
    }

    double getDistance() {
        return distance;
    }

    Point* first;
    Point* second;

private:
    double distance;
};

class TSP {
public:

    static void optimize(vector<Point*> &history);

    static void dft(vector<Point*> &result, Point* root, map<Point *, set<Point *>> &map);

    static void
    pushEdge(boost::heap::fibonacci_heap<Edge>& heap, Point* point, KDTree& tree, map<Point *, boost::heap::fibonacci_heap<Edge>::handle_type>& map);
};


#endif //PLUME_EXPERIMENTS_TSP_H
