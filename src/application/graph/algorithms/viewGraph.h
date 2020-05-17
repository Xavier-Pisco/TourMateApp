#ifndef SRC_VIEWGRAPH_H
#define SRC_VIEWGRAPH_H

#include "../Graph.h"
#include "../../../lib/GraphViewer/graphviewer.h"
#include "../../containers/GraphClasses.h"
#include "../../Application.h"

template<class T, class P>
void Graph<T, P>::viewGraph() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
    }
    graphViewer = new GraphViewer(600, 600, false);
    graphViewer->createWindow(600, 600);

    if (minCoords.first == 0 && minCoords.second == 0 && maxCoords.first == 0 && maxCoords.second == 0) setMaxMinCoords();

    int i = 1;
    for (Vertex<VertexInfoXML, Road> * v : vertexSet) {
        v->graphViewerID = i;
        graphViewer->addNode(v->graphViewerID, Application::lonToX(v->info.getXMLNodeAttributes().at("lon"), 600, minCoords, maxCoords), Application::latToY(v->info.getXMLNodeAttributes().at("lat"), 600, minCoords, maxCoords));
        i++;
    }

    i = 1;
    for (Vertex<VertexInfoXML, Road> * v : vertexSet) {
        for (Edge<VertexInfoXML, Road> &e : v->adj) {
            graphViewer->addEdge(i, v->graphViewerID, e.dest->graphViewerID, EdgeType::DIRECTED);
            i++;
        }
    }

    graphViewer->rearrange();
}

template<class T, class P>
void Graph<T, P>::closeView() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
        graphViewer = nullptr;
    }
}


#endif //SRC_VIEWGRAPH_H