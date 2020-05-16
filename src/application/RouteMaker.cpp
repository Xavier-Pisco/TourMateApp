#include "RouteMaker.h"
#include "Menu.h"
#include "Drawer.h"
#include <dirent.h>

void RouteMaker::start() {

    Menu menu;
    menu.addOption("return");


    Drawer::drawTitle("Choose a map");
    cout << endl;

    vector<string> maps = getAvailableMaps();
    menu.addExtraInput(maps);

    if (maps.empty()) cout << "No maps were found" << endl;

    menu.draw();
    string res;
    int opt = menu.getResponse("\n\nInsert an option:", res);
    if (opt == -1) {
        openMap(res);
        getRouteInfo();
        return;
    }
    switch(opt) {
        case 0:
        default:
            break;
    }
}

vector<string> RouteMaker::getAvailableMaps() const {
    vector<string> res;
    string path = "../maps";

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

}

void RouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
}
