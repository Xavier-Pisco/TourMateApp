#include "MapContainer.h"

SimpleMapContainer::SimpleMapContainer(string &map) : MapContainer<VertexInfoTXT>() {
    graph = Converter::getGraphFromTXTFile(map, nodes);
    graph->setOriginalVertexSet();
    graphViewer = new GraphViewerCustom<VertexInfoTXT>(graph);
    if (graph->minCoords.first == 0 && graph->minCoords.second == 0 && graph->maxCoords.first == 0 && graph->maxCoords.second == 0)
        setGraphMaxMinCoords();
}

const map<long, Vertex<VertexInfoTXT>*> &SimpleMapContainer::getNodes() const {
    return nodes;
}


