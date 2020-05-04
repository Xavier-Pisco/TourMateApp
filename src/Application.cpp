#include <iostream>
#include "Application.h"
#include "lib/GraphViewer/graphviewer.h"

Application::Application(MODE mode) {
    this->mode = mode;
}

void Application::start() {
    graph = Converter::getGraphFromOSMFile("../maps/zona_feup_hsj.osm");

    if (mode == DEBUG) viewGraph();

    int count = 1;
    for (const StreetIntersection& si : graph->dfs()) {
        if (si.getXMLTags().find("name") != si.getXMLTags().end()) {
            cout << "### Place" << endl;
            for (const auto& i : si.getXMLTags()) {
                cout << i.first << " : " << i.second << endl;
            }
            cout << endl << endl;
        }
        count++;
    }
}

void Application::viewGraph() {
    graphViewer = new GraphViewer(600, 600, true);
    graphViewer->createWindow(600, 600);

    int i = 1;
    for (Vertex<StreetIntersection, Road> * v : graph->vertexSet) {
        v->graphViewerID = i;
        i++;
    }

    i = 1;
    for (Vertex<StreetIntersection, Road> * v : graph->vertexSet) {
        graphViewer->addNode(v->graphViewerID);
        for (Edge<StreetIntersection, Road> e : v->adj) {
            graphViewer->addEdge(i, v->graphViewerID, e.dest->graphViewerID, EdgeType::DIRECTED);
            i++;
        }
    }

    graphViewer->rearrange();
}
