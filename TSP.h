#ifndef PLUME_EXPERIMENTS_TSP_H
#define PLUME_EXPERIMENTS_TSP_H

#include <vector>
#include <set>
#include <map>
#include "Point.h"
#include "KDTree.h"
#include <iostream>


using namespace std;

typedef pair<Point*, Point*> Edge;

class TSP {
public:

    static void minimize(vector<Point>& history);

    static Edge findMinimumEdge(set<Point*> &included, KDTree &nearestTree);

    static vector<Point*> dft(Point* root, map<Point *, set<Point *>> map);
};


#endif //PLUME_EXPERIMENTS_TSP_H
