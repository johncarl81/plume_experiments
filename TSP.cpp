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

vector<Point*> TSP::pathDFS(map<Point *, LineSegment *> &segmentEndpoints, LineSegment *root, map<Point *, set<Point *>> &map) {

    vector<Point*> result;

    stack<Point*> pointStack;

    if(map.count(root->getStartPtr()) > 0) {
        pointStack.push(root->getEndPtr());
    } else if (map.count(root->getEndPtr()) > 0) {
        pointStack.push(root->getStartPtr());
    } else {
        cout << "Not found!?";
    }

    while(!pointStack.empty()) {
        Point* currentStart = pointStack.top();
        pointStack.pop();

        result.push_back(currentStart);
        LineSegment* segment = segmentEndpoints[currentStart];
        Point* currentEnd = segment->getStartPtr() == currentStart? segment->getEndPtr() : segment->getStartPtr();
        result.push_back(currentEnd);

        if(map.count(currentStart) > 0) {
            for (Point *point : map[currentStart]) {
                pointStack.push(point);
            }
        }

        if(map.count(currentEnd) > 0) {
            for (Point *point : map[currentEnd]) {
                pointStack.push(point);
            }
        }
    }

    return result;
}

class SegmentDistance: public DistanceCalculation {
public:

    explicit SegmentDistance(map<Point*, LineSegment*> segmentEndpoints): segmentEndpoints(segmentEndpoints){};

    double distance(Point &one, Point &two) override {
        LineSegment* segmentTwo = segmentEndpoints[&two];

        Point* adjacentTwo = segmentTwo->getStartPtr() == &two ? segmentTwo->getEndPtr() : segmentTwo->getStartPtr();

        return (one - *adjacentTwo).length() + segmentTwo->length();
    }

private:
    map<Point*, LineSegment*> segmentEndpoints;
};

vector<Point*> TSP::optimize(vector<LineSegment *> &history) {

    map<Point*, LineSegment*> segmentEndpoints;

    for(auto segment : history) {
        segmentEndpoints[segment->getStartPtr()] = segment;
        segmentEndpoints[segment->getEndPtr()] = segment;
    }

    boost::heap::fibonacci_heap<Edge> queue;
    set<Point*> included;
    map<Point*, set<Point*>> minimumSpanningTree;
    map<Point*, boost::heap::fibonacci_heap<Edge>::handle_type> heapHandles;

    LineSegment* first = history.back();

    KDTree nearestTree;
    for (auto & historyIterator : history) {
        nearestTree.insert(historyIterator->getStartPtr());
        nearestTree.insert(historyIterator->getEndPtr());
    }

    nearestTree.remove(first->getStartPtr());
    nearestTree.remove(first->getEndPtr());

    included.insert(first->getStartPtr());
    included.insert(first->getEndPtr());

    pushEdge(queue, first->getStartPtr(), nearestTree, heapHandles);
    pushEdge(queue, first->getEndPtr(), nearestTree, heapHandles);

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
            LineSegment* segment = segmentEndpoints[smallestEdge.second];
            included.insert(segment->getStartPtr());
            included.insert(segment->getEndPtr());

            nearestTree.remove(segment->getStartPtr());
            nearestTree.remove(segment->getEndPtr());

            LineSegment* secondSegment = segmentEndpoints[smallestEdge.second];

            pushEdge(queue, smallestEdge.first, nearestTree, heapHandles);
            pushEdge(queue, secondSegment->getStartPtr(), nearestTree, heapHandles);
            pushEdge(queue, secondSegment->getEndPtr(), nearestTree, heapHandles);
        } else {
            pushEdge(queue, smallestEdge.first, nearestTree, heapHandles);
        }
    }

    return pathDFS(segmentEndpoints, first, minimumSpanningTree);
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

    for (auto historyEntry : history) {
        nearestTree.insert(historyEntry);
    }

    nearestTree.remove(first);
    included.insert(first);
    pushEdge(queue, first, nearestTree, heapHandles);

    while(!queue.empty()) {
//        cout << "Size: " << queue.size() << endl;
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
            pushEdge(queue, smallestEdge.first, nearestTree, heapHandles);
        }
    }

    // Depth first search effectively implements the triangle inequality ordering of the MST.
    dft(history, first, minimumSpanningTree);
}

void TSP::pushEdge(boost::heap::fibonacci_heap<Edge>& queue, Point* point, KDTree& nearestTree, map<Point *, boost::heap::fibonacci_heap<Edge>::handle_type>& map) {

    PointDistance nearest = nearestTree.nearest(point);
    // Only add the new point if it is connected to the current tree with a longer edge.
    if(nearest.second != INFINITY) {
        if (map.count(nearest.first) > 0) {
            boost::heap::fibonacci_heap<Edge>::handle_type handle = map.at(nearest.first);
            if (handle.node_->value.getDistance() > nearest.second) {
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
}
