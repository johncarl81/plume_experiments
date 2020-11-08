#include "TSP.h"

vector<Point*> TSP::dft(Point* root, map<Point *, set<Point *>> map) {

    vector<Point*> result;
    if(root != nullptr) {
        result.push_back(root);
        for(Point* point : map[root]) {
            vector<Point*> dftresult = dft(point, map);

            for(Point* dftpoint : dftresult) {
                result.push_back(dftpoint);
            }
        }
    }
    return result;
}

void TSP::minimize(vector<Point> &history) {

//    for(Point point : history) {
//        cout << "Point (" << point.getX() << "," << point.getY() << ")" << endl;
//    }

    double distance = 0;
    for(int i = 0; i < history.size() - 1; i++) {
        distance += ((history[i]) - (history[i + 1])).length();
    }

    cout << "Starting distance: " << distance << endl;

    set<Point*> included;
    included.insert(&history.front());
    map<Point*, set<Point*>> minimumSpanningTree;

    cout << "Building tree..." << endl;
    KDTree nearestTree;
    for(Point &point : history) {
        nearestTree.insert(&point);
    }
    nearestTree.remove(&history.front());

    cout << "Finding nearest..." << endl;
    while(included.size() < history.size()) {
        cout << "Size: " << included.size() << endl;
        Edge next = findMinimumEdge(included, nearestTree);

        if(minimumSpanningTree.count(next.first) == 0) {
            minimumSpanningTree[next.first] = set<Point*>();
        }
        minimumSpanningTree[next.first].insert(next.second);

        included.insert(next.second);

        nearestTree.remove(next.second);
    }

    cout << "Running dft..." << endl;
    vector<Point*> result = dft(&history.front(), minimumSpanningTree);

    cout << "size: " << result.size() << endl;
    distance = 0;
    for(int i = 0; i < result.size() - 1; i++) {
        distance += ((*result[i]) - (*result[i + 1])).length();
    }
//
//    for(Point* point : result) {
//        cout << "Point (" << point->getX() << "," << point->getY() << ")" << endl;
//    }

    cout << "Ending distance: " << distance << endl;

}

Edge TSP::findMinimumEdge(set<Point*> &included, KDTree& nearestTree) {

    PointDistance minimum(nullptr, INFINITY);
    Point* from = nullptr;

    for(Point* point : included) {

        PointDistance nearby = nearestTree.nearest(point);

        if(nearby.second < minimum.second) {
            minimum = nearby;
            from = point;
        }
    }

    return Edge(from, minimum.first);
}
