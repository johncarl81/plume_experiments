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

class DistanceCalculation {
public:
    virtual double distance(Point &one, Point &two) = 0;
};

class L2Norm : public DistanceCalculation {
public:
    double distance(Point &one, Point &two) override {
        return (one - two).length();
    }
};

class KDTree {

public:

    KDTree(): distanceCalculation(new L2Norm()){};

    KDTree(DistanceCalculation* calculation) : distanceCalculation(calculation) {};

    ~KDTree();

    void insert(Point* point);

    void remove(Point* point);

    PointDistance nearest(Point* point);

private:
    KDNode* tree = nullptr;
    DistanceCalculation* distanceCalculation;

    static void insert(KDNode* parent, Point* point);

    static void remove(KDNode* parent, Point* point);

    PointDistance nearest(KDNode* parent, Point* point);

    static double getAxisDistance(KDNode* parent, Point* point);
};


#endif //PLUME_EXPERIMENTS_KDTREE_H
