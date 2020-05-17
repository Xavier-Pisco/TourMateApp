#ifndef SRC_DIJKSTRA_H
#define SRC_DIJKSTRA_H

#include "../Graph.h"
#include "../MutablePriorityQueue.h"
#include <cfloat>

template<class T, class P>
void Graph<T, P>::dijkstra(Vertex<T, P> * s) {
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->dist = DBL_MAX;
        (*it)->path = NULL;
    }

    if (s == NULL) return;

    s->dist = 0;

    MutablePriorityQueue<Vertex<T, P>> vertexQueue;

    vertexQueue.insert((Vertex<T, P> *) s);

    while(!vertexQueue.empty()) {
        s = vertexQueue.extractMin();

        for (Edge<T, P> a : s->adj) {
            if (a.dest->getDist() > s->getDist() + a.weight) {
                double oldDist = a.dest->getDist();
                a.dest->dist = s->getDist() + a.weight;
                a.dest->path = s;
                if (oldDist == DBL_MAX) {
                    vertexQueue.insert((Vertex<T, P> *) a.dest);
                } else {
                    vertexQueue.decreaseKey(a.dest);
                }
            }
        }
    }
}

template<class T, class P>
pair<vector<P>, double> Graph<T, P>::getPathToFromDijkstra(Vertex<T, P> * s, Vertex<T, P> * d) {
    // Note, this requires dijkstra algorithm to have been executed starting on vertex *s
    vector<P> path;
    Vertex<T, P> * prevVertex, * currVertex = d;

    if (currVertex->dist == DBL_MAX) return pair<vector<P>, int>(vector<P>(), 0); // Impossible to reach

    while ((prevVertex = currVertex->path) != nullptr) {
        bool found = false;
        for (Edge<T, P> &edge : prevVertex->adj) {
            if (edge.dest == currVertex) {
                P road = edge.info;
                if (path.begin() == path.end() || !(*path.begin() == road)) {
                    path.insert(path.begin(), road);
                }
                found = true;
            }
        }
        if (!found) throw exception();
        currVertex = prevVertex;
    }

    return pair<vector<P>, double>(path, d->dist);
}


#endif //SRC_DIJKSTRA_H
