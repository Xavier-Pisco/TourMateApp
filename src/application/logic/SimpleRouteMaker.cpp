#include "RouteMaker.h"



SimpleRouteMaker::SimpleRouteMaker(string map) {
    mapContainer = new SimpleMapContainer(map);
    graphAnalyzer = new GraphAnalyzer<VertexInfoTXT>(mapContainer->getGraph());

    mapContainer->getGraphViewer()->viewGraph();
}


void SimpleRouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Vertex<VertexInfoTXT> * vx;

    cout << endl;
    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer, false);
    user.setOrigin(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer, false, true); // in future it will not be mandatory
    user.setDestination(vx);

    cout << endl;
    int time = UserInput::getInt("Available time in minutes: ", 0);
    user.setAvailability(time);

    cout << endl; Drawer::drawTitle("Preferences", 0, 40, true, "left"); cout << endl;
    string opt;
    while((opt=UserInput::getPreference())!="done"){
        user.addPreference(opt);
    }

    makeRoute();
}

void SimpleRouteMaker::makeRoute() {

    map<long, Vertex<VertexInfoTXT>*> nodes = mapContainer->getNodes();

    mapContainer->getGraph()->dijkstra(user.getOrigin());
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination());

    auto r = new Route<VertexInfoTXT>(p.first);

    cout << endl << "Distance = " << p.second << "km" << endl << endl;

    cout << r->getStringRepresentation();

    bool conf = UserInput::getConfirmation("Do you want to see the route in the graph viewer? \nIf you say yes, and you've closed the graph viewer window, the program will freeze.");
    if (conf) mapContainer->getGraphViewer()->viewRoute(r);
}

void SimpleRouteMaker::openGraphAnalyzer() {
    graphAnalyzer->menu();
}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
