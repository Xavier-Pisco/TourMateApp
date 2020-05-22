#include "RouteMaker.h"



SimpleRouteMaker::SimpleRouteMaker(string map) {
    mapContainer = new SimpleMapContainer(map);

    mapContainer->getGraphViewer()->viewGraph();
}

void SimpleRouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Vertex<VertexInfoTXT> *origin;
    Vertex<VertexInfoTXT> *dest;

    cout << endl;
    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    origin = UserInput::getVertex(mapContainer, false);
    //user.setOrigin(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    dest = UserInput::getVertex(mapContainer, false, true); // in future it will not be mandatory

    /*cout << endl;
    float time = UserInput::getFloat("Available time in minutes: ");
    user.setAvailability(time);

    cout << endl; Drawer::drawTitle("Preferences", 0, 40, true, "left"); cout << endl;
    string opt;
    while((opt=UserInput::getPreference())!="done"){
        user.addPreference(opt);
    }*/

    makeRoute(origin, dest);
}

void SimpleRouteMaker::makeRoute(Vertex<VertexInfoTXT> * v1, Vertex<VertexInfoTXT> *v2) {
    auto r = new Route<VertexInfoTXT>();
    map<long, Vertex<VertexInfoTXT>*> nodes = mapContainer->getNodes();

    mapContainer->getGraph()->dijkstra(v1);
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = mapContainer->getGraph()->getPathToFromDijkstra(v1, v2);
    r->setRoutePoints(p.first);
    cout << "Distance = " << p.second << endl;

    mapContainer->getGraphViewer()->viewRoute(r);
}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
