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

    Menu menu;
    vector<string> availableCategories = mapContainer->getAvailableCategories();
    menu.addExtraInput(availableCategories);

    menu.addOption("cancel");
    menu.addOption("done");

    bool done = false;
    while(!done) {
        cout << endl;
        menu.draw("");

        string res;
        int opt = menu.getResponse("Choose an option", res);

        switch(opt) {
            case -1:
                user.addPreference(res);
                break;
            case 0:
                throw CancelInput();
            case 1:
                done = true;
            default:
                break;
        }
    }

    makeRoute();
}

void SimpleRouteMaker::makeRoute() {

    map<long, Vertex<VertexInfoTXT>*> nodes = mapContainer->getNodes();

    mapContainer->getGraph()->dijkstra(user.getOrigin());
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination());

    user.setRoute(new Route<VertexInfoTXT>(p.first, p.second));

    displayRoute();
}

void SimpleRouteMaker::displayRoute() {
    cout << endl << "Distance = " << user.getRoute()->getDist() << "km" << endl << endl;

    cout << user.getRoute()->getStringRepresentation();

    bool conf = UserInput::getConfirmation("Do you want to see the route in the graph viewer? \nIf you say yes, and you've closed the graph viewer window, the program will freeze.");
    if (conf) mapContainer->getGraphViewer()->viewRoute(user.getRoute());
}

void SimpleRouteMaker::openGraphAnalyzer() {
    graphAnalyzer->menu();
}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
