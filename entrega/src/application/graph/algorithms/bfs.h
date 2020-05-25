#ifndef SRC_BFS_H
#define SRC_BFS_H

#include "../Graph.h"

/**
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
vector<Vertex<T>*> Graph<T>::bfs(Vertex<T>* s) const {
    vector<Vertex<T>*> res;
    for (typename vector<Vertex<T> *>::const_iterator it = originalVertexSet.begin(); it != originalVertexSet.end(); it++) {
        (*it)->visited = false;
    }
    queue<Vertex<T> *> vertexQueue;

    if (s == NULL) return vector<Vertex<T>*>();

    vertexQueue.push(s);
    s->visited = true;

    while(!vertexQueue.empty()) {
        s = vertexQueue.front();
        vertexQueue.pop();
        res.push_back(s);
        for (Edge<T> * a : s->adj) {
            if (!a->dest->visited) {
                vertexQueue.push(a->dest);
                a->dest->visited = true;
            }
        }
    }
    return res;
}

#endif //SRC_BFS_H
