#include "MapContainer.h"

SimpleMapContainer::SimpleMapContainer(string &map) : MapContainer<VertexInfoTXT>() {
    graph = Converter::getGraphFromTXTFile(map, nodes);
    graph->setOriginalVertexSet();
    graphViewer = new GraphViewerCustom<VertexInfoTXT>(graph);
    if (graph->getMinCoords().first == 0 && graph->getMinCoords().second == 0 && graph->getMaxCoords().first == 0 && graph->getMaxCoords().second == 0)
        setGraphMaxMinCoords();
}

const map<long, Vertex<VertexInfoTXT>*> &SimpleMapContainer::getNodes() const {
    return nodes;
}


