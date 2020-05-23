#ifndef SRC_FINDARTICULATIONPOINTS_H
#define SRC_FINDARTICULATIONPOINTS_H

#include "../Graph.h"

/*
 * THIS IS ONLY FOR UNDIRECTED GRAPHS?
 * */

template<class T>
vector<Vertex<T>*> Graph<T>::findArticulationPoints() {  // This is for connected graph
    vector<Vertex<T>*> res;

    if (vertexSet.empty()) return res;

    for (auto v : vertexSet) {
        v->visited = false;
        v->parent = nullptr;
    }

    counter = 1;
    findArt(vertexSet.at(0), res);

    return res;
}

template<class T>
void Graph<T>::findArt(Vertex<T> * vx, vector<Vertex<T>*> &v) {
    vx->visited = true;
    counter++;
    vx->low = vx->num = counter;
    for (Edge<T> * e : vx->adj) {
        Vertex<T> * adjVx = e->dest;
        if (!adjVx->visited) {
            adjVx->parent = vx;
            findArt(adjVx, v);
            vx->low = min(vx->low, adjVx->low);
            if (adjVx->low >= vx->num) {
                v.push_back(vx);
            }
        } else {
            if (vx->parent != adjVx) vx->low = min(vx->low, adjVx->num);
        }
    }
}

#endif //SRC_FINDARTICULATIONPOINTS_H
