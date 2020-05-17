#ifndef SRC_DFS_H
#define SRC_DFS_H

#include "../Graph.h"

/**
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T, class P>
vector<T> Graph<T, P>::dfs() const {
    vector<T> res;
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if (!(*it)->visited) {
            dfsVisit(*it, res);
        }
    }
    return res;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T, class P>
void Graph<T, P>::dfsVisit(Vertex<T, P> *v, vector<T> & res) const {
    v->visited = true;
    res.push_back(v->info);
    for (Edge<T, P> a : v->adj) {
        if (!a.dest->visited) {
            dfsVisit(a.dest, res);
        }
    }
}

#endif //SRC_DFS_H
