#include "GraphViewerCustom.h"

GraphViewerCustom::GraphViewerCustom(Graph<VertexInfoXML> * graph) {
    this->graph = graph;
}

void GraphViewerCustom::viewGraph() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
    }
    graphViewer = new GraphViewer(600, 600, false);
    graphViewer->createWindow(600, 600);

    int i = 1;
    for (Vertex<VertexInfoXML> * v : graph->vertexSet) {
        v->graphViewerID = i;
        graphViewer->addNode(v->graphViewerID, Application::lonToX(v->info.getXMLAttributes().at("lon"), 600, graph->minCoords, graph->maxCoords), Application::latToY(
                v->info.getXMLAttributes().at("lat"), 600, graph->minCoords, graph->maxCoords));
        i++;
    }

    i = 1;
    for (Vertex<VertexInfoXML> * v : graph->vertexSet) {
        for (Edge<VertexInfoXML> * e : v->adj) {
            graphViewer->addEdge(i, v->graphViewerID, e->dest->graphViewerID, EdgeType::DIRECTED);
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

