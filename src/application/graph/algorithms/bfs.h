#ifndef SRC_BFS_H
#define SRC_BFS_H

#include "../Graph.h"

/**
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T, class P>
vector<T> Graph<T, P>::bfs(const T & source) const {
    vector<T> res;
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    queue<Vertex<T, P> *> vertexQueue;

    Vertex<T, P> * s = findVertex(source);
    if (s == NULL) return vector<T>();

    vertexQueue.push(s);
    s->visited = true;

    while(!vertexQueue.empty()) {
        s = vertexQueue.front();
        vertexQueue.pop();
        res.push_back(s->info);
        for (Edge<T, P> a : s->adj) {
            if (!a.dest->visited) {
                vertexQueue.push(a.dest);
                a.dest->visited = true;
            }
        }
    }
    return res;
}

#endif //SRC_BFS_H
