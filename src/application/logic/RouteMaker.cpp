#include "RouteMaker.h"

void RouteMaker::start() {
    Menu menu;
    menu.addOption("return");


    Drawer::drawTitle("Choose a map");
    cout << endl;

    string path = "../maps/";

    vector<string> maps = getAvailableMaps(path);
    menu.addExtraInput(maps);

    if (maps.empty()) cout << "No maps were found" << endl;

    menu.draw();
    string res;
    cout << endl << endl;
    int opt = menu.getResponse("Insert an option:", res);
    if (opt == -1) {
        path = path + res;
        openMap(path);
        while(true) {
            try {
                getRouteInfo();
            } catch(CancelInput &c) {break;}
        }
        return;
    }
    switch(opt) {
        case 0:
        default:
            break;
    }
}

vector<string> RouteMaker::getAvailableMaps(string & path) const {
    vector<string> res;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("../maps")) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            string name(ent->d_name);
            if (name != "." && name != "..") {
                res.push_back(name);
            }
        }
        closedir (dir);
    }

    return res;
}

void RouteMaker::openMap(string &map) {
    mapContainer = new MapContainer(map);

    mapContainer->getGraphViewer()->viewGraph();

    UserInput::getLine("Press ENTER to close graph. Note: If you close on the 'x' of the window it will shutdown the program");
    mapContainer->getGraphViewer()->closeView();
}

void RouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Vertex<VertexInfoXML> *vx;

    cout << endl;
    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer);
    cout << "Vertex id = " << vx->getInfo().getID() << endl;
    user.setOrigin(vx);
    mapContainer->setReachableVertexSet(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    auto v2 = UserInput::getVertex(mapContainer, false);
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

void RouteMaker::makeRoute() {
    mapContainer->getGraph()->dijkstra(user.getOrigin());
    if (user.getDestination() != nullptr) cout << "the distance in km is " << mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination()).second << endl;

    // this divides into two cases: route with a predefined destination and route with a non-predefined destination
}

RouteMaker::~RouteMaker() {
    delete mapContainer;
}
