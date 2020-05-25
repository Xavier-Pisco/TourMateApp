#ifndef SRC_DFS_H
#define SRC_DFS_H

#include "../Graph.h"

/**
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<Vertex<T> *> Graph<T>::dfs() const {
    vector<Vertex<T>*> res;
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        if (!(*it)->visited) {
            dfsVisit(*it, res);
        }
    }
    return res;
}

template <class T>
stack<Vertex<T> *> Graph<T>::dfsToStack() const {
    stack<Vertex<T>*> res;
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    for (auto it = vertexSet.begin(); it != vertexSet.end(); it++) {
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
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, vector<Vertex<T>*> & res) const {
    v->visited = true;
    for (Edge<T> * a : v->adj) {
        if (!a->dest->visited) {
            dfsVisit(a->dest, res);
        }
    }
    res.push_back(v);
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, stack<Vertex<T>*> & res) const {
    v->visited = true;
    for (Edge<T> * a : v->adj) {
        if (!a->dest->visited) {
            dfsVisit(a->dest, res);
        }
    }
    res.push(v);
}

#endif //SRC_DFS_H
