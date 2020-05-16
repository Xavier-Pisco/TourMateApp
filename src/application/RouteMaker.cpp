#include "RouteMaker.h"
#include "../graph/viewGraph.h"
#include "../input/UserInput.h"

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
    graph = Converter::getGraphFromOSMFile(map);
    graph->viewGraph();
    UserInput::getLine("Press ENTER to close graph. Note: If you close on the 'x' of the window it will shutdown the program");
}

void RouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
}

RouteMaker::~RouteMaker() {
    delete graph;
}
