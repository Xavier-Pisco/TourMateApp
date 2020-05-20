#include "MapContainer.h"

OSMapContainer::OSMapContainer(string &map) : MapContainer<VertexInfoXML>()  {

    graph = Converter::getGraphFromOSMFile(map, roads, placesWays, placesNodes);
    graph->setOriginalVertexSet();
    graphViewer = new GraphViewerCustom<VertexInfoXML>(graph);
    if (graph->minCoords.first == 0 && graph->minCoords.second == 0 && graph->maxCoords.first == 0 && graph->maxCoords.second == 0)
        setGraphMaxMinCoords();
}

void OSMapContainer::findTagName(Vertex<VertexInfoXML> * v, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const {
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


void OSMapContainer::findTagName(const Coords &coords, const map<string, string> &tags, priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> &vertexWithEditDist, string &name) const {
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

vector<VertexNameEditDist> OSMapContainer::getPlacePossibilitiesWithName(string &name) const {
    priority_queue<VertexNameEditDist, vector<VertexNameEditDist>, greater<>> vertexWithEditDist;
    vector<VertexNameEditDist> v;

    for (Vertex<VertexInfoXML> *vx : graph->getVertexSet()) { // checks for the vertex name
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

const vector<WayInfoXML *> &OSMapContainer::getPlacesWays() const {
    return placesWays;
}

const vector<VertexInfoXML> &OSMapContainer::getPlacesNodes() const {
    return placesNodes;
}

const vector<WayInfoXML *> &OSMapContainer::getRoads() const {
    return roads;
}

