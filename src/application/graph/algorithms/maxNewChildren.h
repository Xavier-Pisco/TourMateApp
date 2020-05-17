#ifndef SRC_MAXNEWCHILDREN_H
#define SRC_MAXNEWCHILDREN_H

#include "../Graph.h"

/**
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T, class P>
int Graph<T, P>::maxNewChildren(const T & source, T &inf) const {
    int res = 0;
    for (typename vector<Vertex<T, P> *>::const_iterator it = vertexSet.begin(); it != vertexSet.end(); it++) {
        (*it)->visited = false;
    }
    queue<Vertex<T, P> *> vertexQueue;

    Vertex<T, P> * s = findVertex(source);
    if (s == NULL) return 0;

    vertexQueue.push(s);
    s->visited = true;

    int count;
    while(!vertexQueue.empty()) {
        count = 0;
        s = vertexQueue.front();
        vertexQueue.pop();
        for (Edge<T, P> a : s->adj) {
            if (!a.dest->visited) {
                vertexQueue.push(a.dest);
                a.dest->visited = true;
                count++;
            }
        }
        if (count > res) {
            res = count;
            inf = s->info;
        }
    }
    return res;
}

#endif //SRC_MAXNEWCHILDREN_H
