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
public:
    explicit SimpleMapContainer(string & map);

    const map<long, Vertex<VertexInfoTXT>*> &getNodes() const;
};


#endif //SRC_MAPCONTAINER_H
