#include "TSP.h"

void TSP::dft(vector<Point*> &result, Point* root, map<Point *, set<Point *>> &map) {

    result.clear();

    stack<Point*> pointStack;
    pointStack.push(root);
    while(!pointStack.empty()) {
        Point* currentRoot = pointStack.top();
        pointStack.pop();

        result.push_back(currentRoot);

        if(map.count(currentRoot) > 0) {
            for (Point *point : map[currentRoot]) {
                pointStack.push(point);
            }
        }
    }
}

void TSP::optimize(vector<Point*> &history) {

    if(history.size() < 2) {
        return;
    }

    boost::heap::fibonacci_heap<Edge> queue;
    set<Point*> included;
    map<Point*, set<Point*>> minimumSpanningTree;
    map<Point*, boost::heap::fibonacci_heap<Edge>::handle_type> heapHandles;

    KDTree nearestTree;
    Point* first = history.back();
    history.pop_back();
    for(Point* point : history) {
        nearestTree.insert(point);
    }
    included.insert(first);
    nearestTree.remove(first);
    pushEdge(queue, first, nearestTree, heapHandles);

    while(!queue.empty()) {
        Edge smallestEdge = queue.top();
        queue.pop();

        // smallestEdge.first is always included
        if(included.count(smallestEdge.second) == 0) {

            // Add the edge to the MST
            if(minimumSpanningTree.count(smallestEdge.first) == 0) {
                minimumSpanningTree[smallestEdge.first] = set<Point*>();
            }
            minimumSpanningTree[smallestEdge.first].insert(smallestEdge.second);
            included.insert(smallestEdge.second);

            // And remove the destination from the searchable set
            nearestTree.remove(smallestEdge.second);

            pushEdge(queue, smallestEdge.first, nearestTree, heapHandles);
            pushEdge(queue, smallestEdge.second, nearestTree, heapHandles);
        } else {
            PointDistance nearest = nearestTree.nearest(smallestEdge.first);
            if(nearest.second != INFINITY) {
                queue.push(Edge(smallestEdge.first, nearest));
            }
        }
    }

    dft(history, first, minimumSpanningTree);
}

void TSP::pushEdge(boost::heap::fibonacci_heap<Edge>& queue, Point* point, KDTree& nearestTree, map<Point *, boost::heap::fibonacci_heap<Edge>::handle_type>& map) {

    PointDistance nearest = nearestTree.nearest(point);
    // Only add the new point if it is connected to the current tree with a longer edge.
    if(map.count(nearest.first) > 0) {
        boost::heap::fibonacci_heap<Edge>::handle_type handle = map.at(nearest.first);
        if(handle.node_->value.getDistance() > nearest.second) {
            // Replace the key if it exists.
            boost::heap::fibonacci_heap<Edge>::const_reference ref(Edge(point, nearest));
            queue.decrease(handle, ref);
        }
    } else {
        // Add a new entry because it doesn't exist.
        boost::heap::fibonacci_heap<Edge>::handle_type handle = queue.push(Edge(point, nearest));
        map[nearest.first] = handle;
    }
}
