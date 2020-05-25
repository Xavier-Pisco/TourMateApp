#include "RouteMaker.h"

OSMRouteMaker::OSMRouteMaker(string map) {
    mapContainer = new OSMapContainer(map);
    graphAnalyzer = new GraphAnalyzer<VertexInfoXML>(mapContainer->getGraph());

    mapContainer->getGraphViewer()->viewGraph();
}

void OSMRouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Drawer::drawTitle("Route Mode", 0, 40, true, "left"); cout << endl;

    Menu m;
    m.addOption("cancel", 10);
    m.addOption("on foot", User<VertexInfoTXT>::FOOT);
    m.addOption("by car", User<VertexInfoTXT>::CAR);

    m.draw("");
    unsigned opt = m.getResponse("Choose an option from the menu:");

    if (opt == 10) throw CancelInput();
    user.setRouteMode((enum User<VertexInfoXML>::routeMode) opt);

    Vertex<VertexInfoXML> *vx;

    cout << endl;
    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer, true);
    cout << "Vertex id = " << vx->getInfo().getID() << endl;
    user.setOrigin(vx);
    mapContainer->setReachableVertexSet(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    auto v2 = UserInput::getVertex(mapContainer, true);
    if (v2 != nullptr) cout << "Vertex id = " << v2->getInfo().getID() << endl;
    user.setDestination(v2);

    /*cout << endl;
    float time = UserInput::getFloat("Available time in minutes: ");
    user.setAvailability(time);*/

    /*cout << endl; Drawer::drawTitle("Preferences", 0, 40, true, "left"); cout << endl;
    string opt;
    while((opt=UserInput::getPreference())!="done"){
        user.addPreference(opt);
    }*/

    makeRoute();
}

void OSMRouteMaker::makeRoute() {
    mapContainer->getGraph()->dijkstra(user.getOrigin());
    if (user.getDestination() != nullptr) cout << "the distance in km is " << mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination()).second << endl;
    pair<vector<pair<Vertex<VertexInfoXML> *, Edge<VertexInfoXML> *>>, double> p;
    try {
        p = mapContainer->getGraph()->getPathToFromDijkstra(
                user.getOrigin(), user.getDestination());
    } catch(ImpossibleToReach & c) {
        cout << "The route you want to make is impossible." << endl;
        return;
    }
    p.first.insert(p.first.begin(), {user.getOrigin(), nullptr});

    auto * r = new Route<VertexInfoXML>(p.first, p.second);

    displayRoute(r);
}

void OSMRouteMaker::displayRoute(Route<VertexInfoXML> * r) {

    Drawer::drawTitle("The Route");

    cout << r->getStringRepresentation(true);

    cout << endl << endl << "Distance = " << r->getDist() << "km" << endl << "It will take approximately " << calculateTimeFromDistance(r->getDist()) << " mins" << endl << endl;

    bool conf = UserInput::getConfirmation("Do you want to see the route in the graph viewer? \nIf you say yes, and you've closed the graph viewer window, the program will freeze.");
    if (conf) mapContainer->getGraphViewer()->viewRoute(r);
}

double OSMRouteMaker::calculateTimeFromDistance(double dist /*in km*/) { // returns in minutes
    if (user.getRouteMode() == User<VertexInfoXML>::CAR) {  // CAR
        // considering avg speed = 9 m/s
        return abs((dist * 1000 / 9.) / 60.);
    } else {  // FOOT
        // avg speed = 1.4m/s = 5km/h
        return abs((dist * 1000 / 1.4) / 60.);
    }
}

void OSMRouteMaker::openGraphAnalyzer() {
    graphAnalyzer->menu();
}

OSMRouteMaker::~OSMRouteMaker() {
    delete mapContainer;
}


