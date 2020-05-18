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

        getRouteInfo();
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
    graph = Converter::getGraphFromOSMFile(map, roads, places);
    graphViewer = new GraphViewerCustom(graph);
    graphViewer->viewGraph();

    UserInput::getLine("Press ENTER to close graph. Note: If you close on the 'x' of the window it will shutdown the program");
    graphViewer->closeView();
}

void RouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Vertex<VertexInfoXML, WayInfoXML> *vx;

    cout << endl;

    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    try {
        vx = UserInput::getVertex(graph, roads, places);
    } catch(CancelInput &c) {
        cout << "You cancelled the operation." << endl;
        return;
    }

    cout << "Vertex id = " << vx->getInfo().getID() << endl;

    user.setOrigin(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    try {
        vx = UserInput::getVertex(graph, roads, places, false);
    } catch(CancelInput &c) {
        cout << "You cancelled the operation." << endl;
        return;
    }

    if (vx != nullptr) cout << "Vertex id = " << vx->getInfo().getID() << endl;

    user.setDestination(vx);

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
    graph->dijkstra(user.getOrigin());
    if (user.getDestination() != nullptr) cout << "the distance in km is " << graph->getPathToFromDijkstra(user.getOrigin(), user.getDestination()).second << endl;

    // this divides into two cases: route with a predefined destination and route with a non-predefined destination
}

RouteMaker::~RouteMaker() {
    delete graph;
    delete graphViewer;
}
