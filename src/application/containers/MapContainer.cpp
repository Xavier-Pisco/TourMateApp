#include "MapContainer.h"

MapContainer::MapContainer(string &map)  {
    graph = Converter::getGraphFromOSMFile(map, roads, placesWays, placesNodes);
    graph->setOriginalVertexSet();
    graphViewer = new GraphViewerCustom(graph);
    if (graph->minCoords.first == 0 && graph->minCoords.second == 0 && graph->maxCoords.first == 0 && graph->maxCoords.second == 0)
        setGraphMaxMinCoords();

}

Graph<VertexInfoXML, WayInfoXML> *MapContainer::getGraph() const {
    return graph;
}

const vector<VertexInfoXML> &MapContainer::getPlacesNodes() const {
    return placesNodes;
}

const vector<WayInfoXML*> &MapContainer::getPlacesWays() const {
    return placesWays;
}

GraphViewerCustom *MapContainer::getGraphViewer() const {
    return graphViewer;
}

const vector<WayInfoXML*> & MapContainer::getRoads() const{
    return roads;
}

void MapContainer::setReachableVertexSet(Vertex<VertexInfoXML, WayInfoXML> *vx) {
    graph->setVertexSet(graph->bfs(vx));
}


void MapContainer::setGraphMaxMinCoords() const {
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


Vertex<VertexInfoXML, WayInfoXML> *MapContainer::getVertexWithCoords(const Coords &c) const {
    pair<Vertex<VertexInfoXML, WayInfoXML> *, double> vertexWithDist;
    vertexWithDist.second = DBL_MAX;

    for (Vertex<VertexInfoXML, WayInfoXML> * v : graph->getVertexSet()) {
        double dist = sqrt( pow(v->getInfo().getLat() - c.first, 2) + pow(v->getInfo().getLon() - c.second, 2) );
        if (dist < vertexWithDist.second) {
            vertexWithDist.first = v;
            vertexWithDist.second = dist;
        }
    }
    return vertexWithDist.first;
}

void MapContainer::findTagName(Vertex<VertexInfoXML, WayInfoXML> * v, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const {
    map<string, string>::const_iterator it;

    if ((it = tags.find("name")) != tags.end()) {
        string a = it->second;
        int editDistance = StringMatcher::getSubstringEditDistance(name, a);
        VertexNameEditDist p;

        if (vertexWithEditDist.empty() || editDistance <= vertexWithEditDist.top().first) {
            p.second.first = v;
        } else p.second.first = nullptr;

        p.second.second = it->second;
        p.first = editDistance;

        vertexWithEditDist.push(p);
    }
}


void MapContainer::findTagName(const Coords &coords, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const {
    map<string, string>::const_iterator it;

    if ((it = tags.find("name")) != tags.end()) {
        string a = it->second;
        int editDistance = StringMatcher::getSubstringEditDistance(name, a);
        VertexNameEditDist p;

        if (vertexWithEditDist.empty() || editDistance <= vertexWithEditDist.top().first) {
            p.second.first = getVertexWithCoords(coords);
        } else p.second.first = nullptr;
        p.second.second = it->second;
        p.first = editDistance;

        vertexWithEditDist.push(p);
    }
}

vector<VertexNameEditDist> MapContainer::getPlacePossibilitiesWithName(string &name) const {
    priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> vertexWithEditDist;
    vector<VertexNameEditDist> v;

    for (Vertex<VertexInfoXML, WayInfoXML> *vx : graph->getVertexSet()) { // checks for the vertex name
        map<string, string> tags = vx->getInfo().getXMLTags();
        findTagName(vx, tags, vertexWithEditDist, name);
    }

    for (const VertexInfoXML &n : placesNodes) {
        const map<string, string>& tags = n.getXMLTags();
        pair<double, double> coords = {n.getLat(), n.getLon()};
        findTagName(coords, tags, vertexWithEditDist, name);
    }

    for (WayInfoXML* r : roads) { // checks for road name
        const map<string, string>& tags = r->getXMLTags();
        findTagName(r->getApprCoords(), tags, vertexWithEditDist, name);
    }

    for (WayInfoXML* p : placesWays) { // checks for place name
        const map<string, string>& tags = p->getXMLTags();
        findTagName(p->getApprCoords(), tags, vertexWithEditDist, name);
    }

    while(!vertexWithEditDist.empty() && v.size() < 5) {
        while(!vertexWithEditDist.empty() && !v.empty() && vertexWithEditDist.top().second.second == v.at(v.size()-1).second.second) {
            vertexWithEditDist.pop();
        }
        v.push_back(vertexWithEditDist.top());
        vertexWithEditDist.pop();
    }
    return v;
}


MapContainer::~MapContainer() {
    delete graph;
    delete graphViewer;
}
