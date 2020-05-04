#include <iostream>
#include <cmath>
#include "Application.h"
#include "lib/GraphViewer/graphviewer.h"

Application::Application(MODE mode) {
    this->mode = mode;
}

void Application::start() {
    graph = Converter::getGraphFromOSMFile("../maps/centro_aliados.osm");
    if (mode == DEBUG) viewGraph();
}

void Application::viewGraph() {
    graphViewer = new GraphViewer(600, 600, false);
    graphViewer->createWindow(600, 600);

    setMaxMinCoords();

    int i = 1;
    for (Vertex<RoadIntersection, Road> * v : graph->vertexSet) {
        v->graphViewerID = i;
        graphViewer->addNode(v->graphViewerID, lonToX(v->info.getXMLNodeAttributes().at("lon"), 600), latToY(v->info.getXMLNodeAttributes().at("lat"), 600));
        i++;
    }

    i = 1;
    for (Vertex<RoadIntersection, Road> * v : graph->vertexSet) {
        for (Edge<RoadIntersection, Road> e : v->adj) {
            graphViewer->addEdge(i, v->graphViewerID, e.dest->graphViewerID, EdgeType::DIRECTED);
            i++;
        }
    }

    graphViewer->rearrange();
}

int Application::latToY(const string &lat, int width) const {
    double latD = strtod(lat.c_str(), nullptr);
    double r = ((latD - minCoords.first)/(maxCoords.first - minCoords.first))*width;
    return (int) round(r);
}

int Application::lonToX(const string &lon, int height) const {
    double lonD = strtod(lon.c_str(), nullptr);
    double r = ((lonD - minCoords.second)/(maxCoords.second - minCoords.second))*height;
    //cout << "x val gotten: " << r << endl;
    return (int) round(r);
}

void Application::setMaxMinCoords() {
    this->maxCoords.first = strtod((graph->vertexSet.at(0)->info.getXMLNodeAttributes().at("lat")).c_str(), nullptr);
    this->maxCoords.second = strtod((graph->vertexSet.at(0)->info.getXMLNodeAttributes().at("lon")).c_str(), nullptr);
    this->minCoords.first = strtod((graph->vertexSet.at(0)->info.getXMLNodeAttributes().at("lat")).c_str(), nullptr);
    this->minCoords.second = strtod((graph->vertexSet.at(0)->info.getXMLNodeAttributes().at("lon")).c_str(), nullptr);

    for (Vertex<RoadIntersection, Road> * v : graph->vertexSet) {
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
