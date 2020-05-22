#include "RouteMaker.h"

OSMRouteMaker::OSMRouteMaker(string map) {
    mapContainer = new OSMapContainer(map);

    mapContainer->getGraphViewer()->viewGraph();
}

void OSMRouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Vertex<VertexInfoXML> *vx;

    cout << endl;
    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer, true);
    cout << "Vertex id = " << vx->getInfo().getID() << endl;
    user.setOrigin(vx);
    mapContainer->setReachableVertexSet(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    auto v2 = UserInput::getVertex(mapContainer, true, false);
    if (v2 != nullptr) cout << "Vertex id = " << v2->getInfo().getID() << endl;
    user.setDestination(v2);

    cout << endl;
    float time = UserInput::getFloat("Available time in minutes: ");
    user.setAvailability(time);

    cout << endl; Drawer::drawTitle("Preferences", 0, 40, true, "left"); cout << endl;
    string opt;
    while((opt=UserInput::getPreference())!="done"){
        user.addPreference(opt);
    }

    makeRoute();
}

void OSMRouteMaker::makeRoute() {
    mapContainer->getGraph()->dijkstra(user.getOrigin());
    if (user.getDestination() != nullptr) cout << "the distance in km is " << mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination()).second << endl;

    // this divides into two cases: route with a predefined destination and route with a non-predefined destination
}

OSMRouteMaker::~OSMRouteMaker() {
    delete mapContainer;
}


