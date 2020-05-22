#ifndef SRC_SCC_H
#define SRC_SCC_H

#include "dfs.h"
#include "../Graph.h"


template<class T>
vector<vector<Vertex<T>*>> Graph<T>::stronglyConnectedComponents() {
    stack<Vertex<T>*> s = dfsToStack();
    Graph<T> * g = getTranspose();
    return dfsFromStack(g, s);
}

template<class T>
vector<vector<Vertex<T>*>> Graph<T>::dfsFromStack(Graph<T> * g, stack<Vertex<T> *> &s) {
    vector<vector<Vertex<T>*>> res;
    for (auto v : g->vertexSet) {
        v->visited = false;
    }
    while(!s.empty()) {
        Vertex<T> * vx = g->findVertex(s.top()->getInfo());
        s.pop();
        if (vx->visited) continue;

        vector<Vertex<T>*> v;
        g->dfsVisit(g->findVertex(s.top()->getInfo()), v);
        res.push_back(v);
    }
    return res;
}

template<class T>
Graph<T> * Graph<T>::getTranspose() const {
    auto * g = new Graph<T>();

    for (Vertex<T> * v : vertexSet) {
        g->addVertex(v->getInfo());
    }

    for (Vertex<T> * v : vertexSet) {
        for (Edge<T> * e : v->adj) {
            if (!g->addEdge(e->dest->getInfo().getID(), v->getInfo().getID(), e->weight)) abort();
        }
    }

    return g;
}


#endif //SRC_SCC_H
