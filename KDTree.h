#ifndef PLUME_EXPERIMENTS_KDTREE_H
#define PLUME_EXPERIMENTS_KDTREE_H

#include <vector>
#include <iostream>
#include "Point.h"

using namespace std;

typedef pair<Point*, double> PointDistance;

class KDNode {

public:
    KDNode(Point *value);
    KDNode(Point *value, bool orientation);

    bool orientation;
    Point* value;
    KDNode* left = nullptr;
    KDNode* right = nullptr;

    bool comparison(Point *point) const;

    void deactivate();

    bool deactivated = false;
};

class KDTree {

public:
    ~KDTree();

    void insert(Point* point);

    void remove(Point* point);

    PointDistance nearest(Point* point);

private:
    KDNode* tree = nullptr;

    static void insert(KDNode* parent, Point* point);

    static void remove(KDNode* parent, Point* point);

    static PointDistance nearest(KDNode* parent, Point* point);

    static double getAxisDistance(KDNode* parent, Point* point);
};


#endif //PLUME_EXPERIMENTS_KDTREE_H
