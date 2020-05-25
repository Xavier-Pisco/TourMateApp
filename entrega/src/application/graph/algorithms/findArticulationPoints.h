#ifndef SRC_FINDARTICULATIONPOINTS_H
#define SRC_FINDARTICULATIONPOINTS_H

#include "../Graph.h"


template<class T>
vector<Vertex<T>*> Graph<T>::findArticulationPoints() {
    vector<Vertex<T>*> res;

    if (vertexSet.empty()) return res;

    for (auto v : vertexSet) {
        v->visited = false;
        v->parent = nullptr;
        v->childNo = 0;
    }

    counter = 1;
    for (auto v : vertexSet) {
        if (!v->visited) findArt(vertexSet.at(0), res, v, true);
        if (v->childNo > 1) res.push_back(v);    // root is only an articulation point if it has more than 1 child
    }

    return res;
}

template<class T>
void Graph<T>::findArt(Vertex<T> * vx, vector<Vertex<T>*> &v, Vertex<T> * rootVx, bool root) {
    vx->visited = true;
    vx->low = vx->num = counter;
    counter++;
    for (Edge<T> * e : vx->adj) {
        Vertex<T> * adjVx = e->dest;
        if (!adjVx->visited) {
            if (vx == rootVx) rootVx->childNo++;   // root is only an articulation point if it has more than 1 child
            adjVx->parent = vx;

            findArt(adjVx, v, rootVx);

            vx->low = min(vx->low, adjVx->low);
            if (adjVx->low >= vx->num && !root) {    // root is only an articulation point if it has more than 1 child
                v.push_back(vx);
            }
        } else {
            if (vx->parent != adjVx) vx->low = min(vx->low, adjVx->num);
        }
    }
}

#endif //SRC_FINDARTICULATIONPOINTS_H
