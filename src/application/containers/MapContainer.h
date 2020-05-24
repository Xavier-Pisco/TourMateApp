#ifndef SRC_MAPCONTAINER_H
#define SRC_MAPCONTAINER_H

#include <cfloat>
#include "../ui/GraphViewerCustom.h"
#include "../input/Converter.h"
#include "../graph/algorithms/bfs.h"
#include "../logic/StringMatcher.h"

typedef pair<double, double> Coords;


template<class T>
class MapContainer {
protected:
    Graph<T> * graph = nullptr;
    GraphViewerCustom<T> * graphViewer = nullptr;

public:

    /**
     * @brief sets the vertexSet to vertexes that are reachable from vx
     * @param vx - the vertex
     */
    void setReachableVertexSet(Vertex<T> * vx);

    Vertex<T> * getVertexWithID(long id) const;

    void setGraphMaxMinCoords() const;

    Vertex<T> *getVertexWithCoords(const Coords &c) const;

    Graph<T> *getGraph() const;

    GraphViewerCustom<T> *getGraphViewer() const;

    ~MapContainer();
};

typedef pair<int, pair<Vertex<VertexInfoXML> *, string>> VertexNameEditDist;

class OSMapContainer : public MapContainer<VertexInfoXML> {
private:
    vector<WayInfoXML*> roads, placesWays;
    vector<VertexInfoXML> placesNodes;

public:
    explicit OSMapContainer(string &map);

    void findTagName(Vertex<VertexInfoXML> * v, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const;

    void findTagName(const Coords &coords, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const;

    vector<VertexNameEditDist> getPlacePossibilitiesWithName(string &name) const;

    const vector<WayInfoXML*> &getPlacesWays() const;

    const vector<VertexInfoXML> &getPlacesNodes() const;

    const vector<WayInfoXML*> & getRoads() const;
};


class SimpleMapContainer : public MapContainer<VertexInfoTXT> {
private:
    map<long, Vertex<VertexInfoTXT>*> nodes;
    map<string, vector<Vertex<VertexInfoTXT>*>> categoryVertexes;
public:
    explicit SimpleMapContainer(string & map, bool performanceTests = false);

    const map<long, Vertex<VertexInfoTXT>*> &getNodes() const;

    vector<string> getAvailableCategories() const;

    vector<Vertex<VertexInfoTXT>*> getVertexesWithCategory(string &c) const;

    vector<Vertex<VertexInfoTXT>*> getPOIVertexes() const;
};


/*  MapContainer  */


template<class T>
Graph<T> *MapContainer<T>::getGraph() const {
    return graph;
}

template<class T>
Vertex<T> * MapContainer<T>::getVertexWithID(long id) const {
    for (auto v : graph->getVertexSet()) {
        if (v->getInfo().getID() == id) return v;
    }
    return nullptr;
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
    pair<Vertex<T> *, double> vertexWithDist;
    vertexWithDist.second = DBL_MAX;

    for (Vertex<T> * v : graph->getVertexSet()) {
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



#endif //SRC_MAPCONTAINER_H
