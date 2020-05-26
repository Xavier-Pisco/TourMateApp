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
    bool notConnected; // true if the graph is not strongly connected

public:

    /**
     * @brief sets the vertexSet to vertexes that are reachable from vx
     * @param vx - the vertex
     */
    void setReachableVertexSet(Vertex<T> * vx);

    /**
     * @brief gets the vertex with the given id
     * @param id
     * @return pointer to the vertex
     */
    Vertex<T> * getVertexWithID(long id) const;

    /**
     * @brief sets the graph's maximum and minimum coords
     */
    void setGraphMaxMinCoords() const;

    /**
     * @brief finds the vertex with the given coords
     * @param c
     * @return pointer to the vertex
     */
    Vertex<T> *getVertexWithCoords(const Coords &c) const;

    /**
     * @brief GET method for the graph
     * @return Graph pointer
     */
    Graph<T> *getGraph() const;

    /**
     * @brief GET method for graphViewer
     * @return graphViewer
     */
    GraphViewerCustom<T> *getGraphViewer() const;

    /**
     * @brief GET method for not connected
     */
    bool getNotConnected();

    /**
     * @brief destructor
     */
    ~MapContainer();
};

typedef pair<int, pair<Vertex<VertexInfoXML> *, string>> VertexNameEditDist;

class OSMapContainer : public MapContainer<VertexInfoXML> {
private:
    vector<WayInfoXML*> roads, placesWays;
    vector<VertexInfoXML> placesNodes;

public:
    explicit OSMapContainer(string &map);

    /**
     * @brief finds the vertex with the name given
     * @param v
     * @param tags
     * @param vertexWithEditDist
     * @param name
     */
    void findTagName(Vertex<VertexInfoXML> * v, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const;

    /**
     * @brief finds the vertex with the name given
     * @param coords
     * @param tags
     * @param vertexWithEditDist
     * @param name
     */
    void findTagName(const Coords &coords, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const;

    /**
     * @brief gets the possible vertexes with the given name
     * @param name
     * @return vector with the results
     */
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
    explicit SimpleMapContainer(string & map, int performanceTests = -1);

    /**
     * @brief GET method for nodes
     */
    const map<long, Vertex<VertexInfoTXT>*> &getNodes() const;

    /**
     * @brief gets the available categories
     * @return vector with the categories
     */
    vector<string> getAvailableCategories() const;

    /**
     * @brief finds the vertexes that have the given category
     * @param c - the category
     * @return vector with the vertexes
     */
    vector<Vertex<VertexInfoTXT>*> getVertexesWithCategory(string &c) const;

    /**
     * @brief finds the POI vertexes
     * @return list of POI vertexes
     */
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

template<class T>
bool MapContainer<T>::getNotConnected() {
    return notConnected;
}


#endif //SRC_MAPCONTAINER_H
