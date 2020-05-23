#include "MapContainer.h"

SimpleMapContainer::SimpleMapContainer(string &map) : MapContainer<VertexInfoTXT>() {
    graph = Converter::getGraphFromTXTFile(map, nodes);
    graph->setOriginalVertexSet();
    graphViewer = new GraphViewerCustom<VertexInfoTXT>(graph);
    if (graph->getMinCoords().first == 0 && graph->getMinCoords().second == 0 && graph->getMaxCoords().first == 0 && graph->getMaxCoords().second == 0)
        setGraphMaxMinCoords();

    for (Vertex<VertexInfoTXT>* v : graph->vertexSet) {
        string c = v->getInfo().getCategory();
        if (!c.empty()) {
            if (categoryVertexes.find(c) != categoryVertexes.end()) {
                categoryVertexes.at(c).push_back(v);
            } else {
                vector<Vertex<VertexInfoTXT>*> vectorVx;
                vectorVx.push_back(v);
                categoryVertexes.insert({c, vectorVx});
            }
        }
    }
}

const map<long, Vertex<VertexInfoTXT>*> &SimpleMapContainer::getNodes() const {
    return nodes;
}

vector<string> SimpleMapContainer::getAvailableCategories() const {
    vector<string> res;
    for (auto p : categoryVertexes) {
        res.push_back(p.first);
    }
    return res;
}

vector<Vertex<VertexInfoTXT>*> SimpleMapContainer::getVertexesWithCategory(string &c) const {
    auto it = categoryVertexes.find(c);
    if (it == categoryVertexes.end()) return vector<Vertex<VertexInfoTXT>*>();
    return it->second;
}

vector<Vertex<VertexInfoTXT>*> SimpleMapContainer::getPOIVertexes() const {
    vector<Vertex<VertexInfoTXT>*> res;
    for (auto p : categoryVertexes) {
        res.insert(res.end(), p.second.begin(), p.second.end());
    }
    return res;
}


