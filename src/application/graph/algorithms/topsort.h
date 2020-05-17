#ifndef SRC_TOPSORT_H
#define SRC_TOPSORT_H

#include "../Graph.h"

/**
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template <class T, class P>
vector<T> Graph<T, P>::topsort() const {
    vector<T> res;

    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->indegree = 0;
    }
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        for (Edge<T, P> a : (*it)->adj) a.dest->indegree++;
    }
    queue<Vertex<T, P> *> C; // vertices with indegree == 0 (candidates)
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if ((*it)->indegree == 0) {
            C.push(*it);
        }
    }

    Vertex<T, P> *v;
    while (!C.empty()) {
        v = C.front();
        C.pop();
        res.push_back(v->info);
        for (Edge<T, P> e : v->adj) {
            e.dest->indegree--;
            if (e.dest->indegree == 0) C.push(e.dest);
        }
    }
    if (res.size() != vertexSet.size()) return vector<T>(); // graph is not a DAG
    return res;
}

#endif //SRC_TOPSORT_H
