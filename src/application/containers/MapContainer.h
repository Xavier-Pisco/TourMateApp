#ifndef SRC_MAPCONTAINER_H
#define SRC_MAPCONTAINER_H

#include <cfloat>

#include "../ui/GraphViewerCustom.h"
#include "../input/Converter.h"
#include "../graph/algorithms/bfs.h"
#include "../logic/StringMatcher.h"

typedef pair<double, double> Coords;
typedef pair<int, pair<Vertex<VertexInfoXML> *, string>> VertexNameEditDist;


class MapContainer {
private:
    Graph<VertexInfoXML> * graph = nullptr;
    GraphViewerCustom * graphViewer = nullptr;
    vector<WayInfoXML*> roads, placesWays;
    vector<VertexInfoXML> placesNodes;

public:
    explicit MapContainer(string &map);

    /**
     * @brief sets the vertexSet to vertexes that are reachable from vx
     * @param vx - the vertex
     */
    void setReachableVertexSet(Vertex<VertexInfoXML> * vx);

    void setGraphMaxMinCoords() const;

    Vertex<VertexInfoXML> *getVertexWithCoords(const Coords &c) const;

    void findTagName(Vertex<VertexInfoXML> * v, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const;

    void findTagName(const Coords &coords, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const;

    vector<VertexNameEditDist> getPlacePossibilitiesWithName(string &name) const;

    Graph<VertexInfoXML> *getGraph() const;

    const vector<VertexInfoXML> &getPlacesNodes() const;

    const vector<WayInfoXML*> &getPlacesWays() const;

    GraphViewerCustom *getGraphViewer() const;

    const vector<WayInfoXML*> & getRoads() const;

    ~MapContainer();
};


#endif //SRC_MAPCONTAINER_H
