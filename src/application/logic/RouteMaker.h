#ifndef SRC_ROUTEMAKER_H
#define SRC_ROUTEMAKER_H

#include <string>
#include <vector>
#include <dirent.h>

#include "../containers/MapContainer.h"
#include "../graph/algorithms/dijkstra.h"
#include "../ui/Menu.h"
#include "../ui/Drawer.h"
#include "../containers/User.h"
#include "../input/UserInput.h"

using namespace std;

template<class T>
class RouteMaker {
protected:
    MapContainer<T> * mapContainer = nullptr;
    User user;
public:
    void start(string &path);
    virtual void openMap(string &map) = 0;
    virtual void getRouteInfo() = 0;
    virtual void makeRoute() = 0;
    vector<string> getAvailableMaps(string & path) const;
};

class SimpleRouteMaker : public RouteMaker<VertexInfoTXT> {
public:
    void openMap(string &map) override;
    void getRouteInfo() override;
    void makeRoute() override;
    ~SimpleRouteMaker();
};

class OSMRouteMaker : public RouteMaker<VertexInfoXML> {
public:
    void openMap(string &map) override;
    void getRouteInfo() override;
    void makeRoute() override;
    ~OSMRouteMaker();
};

/*  RouteMaker  */


template<class T>
void RouteMaker<T>::start(string &path) {
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

template<class T>
vector<string> RouteMaker<T>::getAvailableMaps(string & path) const {
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



#endif //SRC_ROUTEMAKER_H
