#include "GraphViewerCustom.h"

GraphViewerCustom::GraphViewerCustom(Graph<VertexInfoXML, WayInfoXML> * graph) {
    this->graph = graph;
}

void GraphViewerCustom::viewGraph() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
    }
    graphViewer = new GraphViewer(600, 600, false);
    graphViewer->createWindow(600, 600);

    if (graph->minCoords.first == 0 && graph->minCoords.second == 0 && graph->maxCoords.first == 0 && graph->maxCoords.second == 0) graph->setMaxMinCoords();

    int i = 1;
    for (Vertex<VertexInfoXML, WayInfoXML> * v : graph->vertexSet) {
        v->graphViewerID = i;
        graphViewer->addNode(v->graphViewerID, Application::lonToX(v->info.getXMLAttributes().at("lon"), 600, graph->minCoords, graph->maxCoords), Application::latToY(
                v->info.getXMLAttributes().at("lat"), 600, graph->minCoords, graph->maxCoords));
        i++;
    }

    i = 1;
    for (Vertex<VertexInfoXML, WayInfoXML> * v : graph->vertexSet) {
        for (Edge<VertexInfoXML, WayInfoXML> &e : v->adj) {
            graphViewer->addEdge(i, v->graphViewerID, e.dest->graphViewerID, EdgeType::DIRECTED);
            i++;
        }
    }

    graphViewer->rearrange();
}


void GraphViewerCustom::closeView() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
        graphViewer = nullptr;
    }
}

GraphViewerCustom::~GraphViewerCustom() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
    }
}

