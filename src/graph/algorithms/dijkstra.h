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


#endif //SRC_DIJKSTRA_H
