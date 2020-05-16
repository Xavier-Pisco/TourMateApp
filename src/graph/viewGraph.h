#ifndef SRC_VIEWGRAPH_H
#define SRC_VIEWGRAPH_H

#include "Graph.h"
#include "../lib/GraphViewer/graphviewer.h"
#include "GraphClasses.h"
#include "../application/Application.h"

template<class T, class P>
void Graph<T, P>::viewGraph() {
    if (graphViewer != nullptr) {
        graphViewer->closeWindow();
        delete graphViewer;
    }
    graphViewer = new GraphViewer(600, 600, false);
    graphViewer->createWindow(600, 600);

    setMaxMinCoords();

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
void Graph<T, P>::setMaxMinCoords() {
    this->maxCoords.first = strtod((vertexSet.at(0)->info.getXMLNodeAttributes().at("lat")).c_str(), nullptr);
    this->maxCoords.second = strtod((vertexSet.at(0)->info.getXMLNodeAttributes().at("lon")).c_str(), nullptr);
    this->minCoords.first = strtod((vertexSet.at(0)->info.getXMLNodeAttributes().at("lat")).c_str(), nullptr);
    this->minCoords.second = strtod((vertexSet.at(0)->info.getXMLNodeAttributes().at("lon")).c_str(), nullptr);

    for (Vertex<VertexInfoXML, Road> * v : vertexSet) {
        double lat = strtod((v->info.getXMLNodeAttributes().at("lat")).c_str(), nullptr);
        double lon = strtod((v->info.getXMLNodeAttributes().at("lon")).c_str(), nullptr);

        if (lat > maxCoords.first) maxCoords.first = lat;
        if (lon > maxCoords.second) maxCoords.second = lon;
        if (lat < minCoords.first) minCoords.first = lat;
        if (lon < minCoords.second) minCoords.second = lon;
    }
    /*cout << "Min coords: " << minCoords.first << ", " << minCoords.second << endl;
    cout << "Max coords: " << maxCoords.first << ", " << maxCoords.second << endl;*/

}


#endif //SRC_VIEWGRAPH_H