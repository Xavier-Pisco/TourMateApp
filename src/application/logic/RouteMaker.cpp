#include "RouteMaker.h"


string RouteMaker::getMapName(string &path) {
    Menu menu;
    menu.addOption("return");


    Drawer::drawTitle("Choose a map");
    cout << endl;

    vector<string> maps = getAvailableMaps(path);
    menu.addExtraInput(maps);

    if (maps.empty()) cout << "No maps were found" << endl;

    menu.draw();
    string res;
    cout << endl << endl;
    int opt = menu.getResponse("Insert an option:", res);
    if (opt == -1) {
        path = path + res;
        return path;
    }
    throw CancelInput();
}


vector<string> RouteMaker::getAvailableMaps(string & path) {
    vector<string> res;

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
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

RouteMaker *RouteMaker::openMap(string &mapDescription) {
    Drawer::drawTitle("Opening a map");
    Menu menu;
    menu.addOption("Cancel");
    menu.addOption("OSM map");
    menu.addOption("Simple TXT map");
    menu.addOption("Grid Graph");

    cout << endl;
    menu.draw();
    cout << endl;
    unsigned opt = menu.getResponse("Choose an option from the menu:");

    string path;
    string mapName;

    switch(opt) {
        case 0:
            break;
        case 1:
            mapDescription = "OSM map ";
            path = "../maps/osm/";
            mapName = getMapName(path);
            mapDescription += mapName;
            return new OSMRouteMaker(mapName);
        case 2:
            path = "../maps/simple/";
            return new SimpleRouteMaker(getMapName(path));
        case 3:
            path = "../maps/gridGraph/";
            return new SimpleRouteMaker(getMapName(path));
        default:
            break;
    }
    return nullptr;
}

