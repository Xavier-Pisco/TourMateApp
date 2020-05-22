#ifndef SRC_GRAPHVIEWERCUSTOM_H
#define SRC_GRAPHVIEWERCUSTOM_H

#include "../../lib/GraphViewer/graphviewer.h"
#include "../Application.h"
#include "../containers/Route.h"

template<class T>
class GraphViewerCustom {
private:
    Graph<T> * graph;
    GraphViewer * graphViewer = nullptr;

public:
    explicit GraphViewerCustom(Graph<T> * graph);
    void viewRoute(Route<T> * route);
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
    for (Vertex<T> * v : graph->vertexSet) {
        v->graphViewerID = i;
        graphViewer->addNode(v->graphViewerID, Application::lonToX(v->info.getLon(), 600, graph->minCoords, graph->maxCoords), Application::latToY(
                v->info.getLat(), 600, graph->minCoords, graph->maxCoords));
        i++;
    }

    i = 1;
    for (Vertex<T> * v : graph->vertexSet) {
        for (Edge<T> * e : v->adj) {
            graphViewer->addEdge(i, v->graphViewerID, e->dest->graphViewerID, EdgeType::DIRECTED);
            i++;
        }
    }

    graphViewer->rearrange();
}

template<class T>
void GraphViewerCustom<T>::viewRoute(Route<T> * route) {
    //graphViewer->closeWindow();
    //graphViewer->createWindow(600, 600);
    vector<Vertex<T>*> vertexes = route->getVertexes();
    for (int i = 0; i < vertexes.size(); i++) {
        Vertex<T> * v = vertexes.at(i);
        if (i == 0) {  // it's the origin

            graphViewer->setVertexColor(v->graphViewerID, CYAN);

        } else if (i == vertexes.size()-1) { // it's the destiny

            graphViewer->setVertexColor(v->graphViewerID, ORANGE);

        } else if (v->getInfo().getCategory() != "") { // it's a POI

            graphViewer->setVertexColor(v->graphViewerID, RED);

        } else { // it's just part of the path

            graphViewer->setVertexColor(v->graphViewerID, GREEN);
        }
    }
    graphViewer->rearrange();

    cout << "Press ENTER to close this route and start another." << endl;
    string s;
    getline(cin, s);
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
