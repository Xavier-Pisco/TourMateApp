#include "MapContainer.h"

template<class T>
Graph<T> *MapContainer<T>::getGraph() const {
    return graph;
}

template<class T>
GraphViewerCustom<T> *MapContainer<T>::getGraphViewer() const {
    return graphViewer;
}

template<class T>
void MapContainer<T>::setReachableVertexSet(Vertex<T> *vx) {
    vector<Vertex<T>*> v;
    v = graph->bfs(vx);
    graph->setVertexSet(v);
    cout << "New vset size: " << v.size() << endl;
}

template<class T>
void MapContainer<T>::setGraphMaxMinCoords() const {
    graph->maxCoords.first = graph->vertexSet.at(0)->info.getLat();
    graph->maxCoords.second = graph->vertexSet.at(0)->info.getLon();
    graph->minCoords.first = graph->vertexSet.at(0)->info.getLat();
    graph->minCoords.second = graph->vertexSet.at(0)->info.getLon();

    for (auto v : graph->vertexSet) {
        double lat = v->info.getLat();
        double lon = v->info.getLon();

        if (lat > graph->maxCoords.first) graph->maxCoords.first = lat;
        if (lon > graph->maxCoords.second) graph->maxCoords.second = lon;
        if (lat < graph->minCoords.first) graph->minCoords.first = lat;
        if (lon < graph->minCoords.second) graph->minCoords.second = lon;
    }

}

template<class T>
Vertex<T> *MapContainer<T>::getVertexWithCoords(const Coords &c) const {
    pair<Vertex<VertexInfoXML> *, double> vertexWithDist;
    vertexWithDist.second = DBL_MAX;

    for (Vertex<VertexInfoXML> * v : graph->getVertexSet()) {
        double dist = sqrt( pow(v->getInfo().getLat() - c.first, 2) + pow(v->getInfo().getLon() - c.second, 2) );
        if (dist < vertexWithDist.second) {
            vertexWithDist.first = v;
            vertexWithDist.second = dist;
        }
    }
    return vertexWithDist.first;
}



template<class T>
MapContainer<T>::~MapContainer() {
    delete graph;
    delete graphViewer;
}
