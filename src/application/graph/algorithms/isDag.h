#ifndef SRC_ISDAG_H
#define SRC_ISDAG_H

#include "../Graph.h"

/**
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */
template <class T, class P>
bool Graph<T, P>::isDAG() const {
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->processing = false;
        (*it)->visited = false;
    }
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if (!(*it)->visited) {
            (*it)->processing = true;
            if (!dfsIsDAG(*it)) return false;
            (*it)->processing = false;
        }
    }
    return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T, class P>
bool Graph<T, P>::dfsIsDAG(Vertex<T, P> *v) const {
    v->visited = true;
    for (Edge<T, P> a : v->adj) {
        if (a.dest->processing) return false;
        if (!a.dest->visited) {
            a.dest->processing = true;
            if (!dfsIsDAG(a.dest)) return false;
            a.dest->processing = false;
        }
    }
    return true;
}

#endif //SRC_ISDAG_H
