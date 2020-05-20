#ifndef SRC_DIJKSTRA_H
#define SRC_DIJKSTRA_H

#include "../Graph.h"
#include "../MutablePriorityQueue.h"
#include <cfloat>
#include "../../exceptions.h"

template<class T>
void Graph<T>::dijkstra(Vertex<T> * s) {
    for (typename vector<Vertex<T> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->dist = DBL_MAX;
        (*it)->path = NULL;
    }

    if (s == NULL) return;

    s->dist = 0;

    MutablePriorityQueue<Vertex<T>> vertexQueue;

    vertexQueue.insert((Vertex<T> *) s);

    while(!vertexQueue.empty()) {
        s = vertexQueue.extractMin();

        for (Edge<T> * a : s->adj) {
            if (a->dest->getDist() > s->getDist() + a->weight) {
                double oldDist = a->dest->getDist();
                a->dest->dist = s->getDist() + a->weight;
                a->dest->path = s;
                if (oldDist == DBL_MAX) {
                    vertexQueue.insert((Vertex<T> *) a->dest);
                } else {
                    vertexQueue.decreaseKey(a->dest);
                }
            }
        }
    }
}

template<class T>
pair<vector<pair<Vertex<T>*, Edge<T> *>>, double> Graph<T>::getPathToFromDijkstra(Vertex<T> * s, Vertex<T> * d) const {
    typedef pair<Vertex<T>*, Edge<T>*> vertexEdgePair;
    // Note, this requires dijkstra algorithm to have been executed starting on vertex *s
    vector<pair<Vertex<T>*, Edge<T> *>> path;
    Vertex<T> * prevVertex, * currVertex = d;

    if (currVertex->dist == DBL_MAX) throw ImpossibleToReach(); // Impossible to reach

    while ((prevVertex = currVertex->path) != nullptr) {
        bool found = false;
        for (Edge<T> * edge : prevVertex->adj) {
            if (edge->dest == currVertex) {
                path.insert(path.begin(), pair<Vertex<T>*, Edge<T>*>(currVertex, edge));
                found = true;
            }
        }
        if (!found) throw exception();
        currVertex = prevVertex;
    }

    return pair<vector<pair<Vertex<T>*, Edge<T> *>>, double>(path, d->dist);
}


#endif //SRC_DIJKSTRA_H
