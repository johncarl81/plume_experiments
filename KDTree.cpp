#include "KDTree.h"

void KDTree::remove(Point* point) {
    remove(tree, point);
}

void KDTree::remove(KDNode* parent, Point* point) {
    if(parent != nullptr) {
        if (parent->value == point) {
            parent->deactivate();
        } else {
            if (parent->comparison(point)) {
                remove(parent->left, point);
            } else {
                remove(parent->right, point);
            }
        }
    }
}

void KDTree::insert(Point* point) {
    if(tree == nullptr) {
        tree = new KDNode(point);
    }
    else {
        insert(tree, point);
    }
}

void KDTree::insert(KDNode* parent, Point* point){
    if(parent->comparison(point)) {
        if(parent->left == nullptr) {
            parent->left = new KDNode(point, !parent->orientation);
        } else {
            insert(parent->left, point);
        }
    } else {
        if(parent->right == nullptr) {
            parent->right = new KDNode(point, !parent->orientation);
        } else {
            insert(parent->right, point);
        }
    }
}

PointDistance KDTree::nearest(Point* point) {
    return nearest(tree, point);
}

double KDTree::getAxisDistance(KDNode* parent, Point* point){
    if(parent->orientation) {
        return abs(parent->value->getY() - point->getY());
    } else {
        return abs(parent->value->getX() - point->getX());
    }
}

PointDistance KDTree::nearest(KDNode* parent, Point* point) {
    double parentDistance;
    if(!parent->deactivated) {
        parentDistance = (*point - *parent->value).length();
    } else {
        parentDistance = INFINITY;
    }

    PointDistance nearestPoint(parent->value, parentDistance);
    if(parent->comparison(point)) {
        if(parent->left != nullptr) {
            nearestPoint = nearest(parent->left, point);
        }
    } else {
        if(parent->right != nullptr) {
            nearestPoint = nearest(parent->right, point);
        }
    }

    // If we're still within a distance of the split, search the other split as well.
    if(nearestPoint.second > getAxisDistance(parent, point)) {

        if(!parent->comparison(point)) {
            if(parent->left != nullptr) {
                PointDistance alternate = nearest(parent->left, point);
                if(alternate.second < nearestPoint.second) {
                    nearestPoint = alternate;
                }
            }
        } else {
            if(parent->right != nullptr) {
                PointDistance alternate = nearest(parent->right, point);
                if(alternate.second < nearestPoint.second) {
                    nearestPoint = alternate;
                }
            }
        }
    }

    if(nearestPoint.second > parentDistance) {
        nearestPoint = PointDistance(parent->value, parentDistance);
    }

    return nearestPoint;
}

KDTree::~KDTree() {
    delete tree;
}


KDNode::KDNode(Point *value) : value(value), orientation(false) {}
KDNode::KDNode(Point *value, bool orientation) : value(value), orientation(orientation) {}

bool KDNode::comparison(Point* point) const {
    if(orientation) {
        return value->getY() < point->getY();
    } else {
        return value->getX() < point->getX();
    }
}

void KDNode::deactivate() {
     deactivated = true;
}
