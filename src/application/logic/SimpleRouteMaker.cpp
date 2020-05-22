#include "RouteMaker.h"



SimpleRouteMaker::SimpleRouteMaker(string map) {
    mapContainer = new SimpleMapContainer(map);
    graphAnalyzer = new GraphAnalyzer<VertexInfoTXT>(mapContainer->getGraph());

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
    cout << "Distance = " << p.second << "km" << endl;


    bool conf = UserInput::getConfirmation("Do you want to see the route in the graph viewer? \nIf you say yes, and you've closed the graph viewer window, the program will freeze.");
    if (conf) mapContainer->getGraphViewer()->viewRoute(r);
}

void SimpleRouteMaker::openGraphAnalyzer() {
    graphAnalyzer->menu();
}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
