#ifndef SRC_GRAPHVIEWERCUSTOM_H
#define SRC_GRAPHVIEWERCUSTOM_H

#include "../../lib/GraphViewer/graphviewer.h"
#include "../Application.h"
#include "../containers/GraphClasses.h"

template<class T>
class GraphViewerCustom {
private:
    Graph<T> * graph;
    GraphViewer * graphViewer = nullptr;

public:
    explicit GraphViewerCustom(Graph<T> * graph);
    void viewGraph();
    void closeView();
    ~GraphViewerCustom();
};

template<class T>
GraphViewerCustom<T>::GraphViewerCustom(Graph<T> * graph) {
    this->graph = graph;
}

template<class T>
void GraphViewerCustom<T>::viewGraph() {
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

template<class T>
void GraphViewerCustom<T>::closeView() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
        graphViewer = nullptr;
    }
}

template<class T>
GraphViewerCustom<T>::~GraphViewerCustom() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
    }
}


#endif //SRC_GRAPHVIEWERCUSTOM_H
