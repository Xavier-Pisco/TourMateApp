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

class RouteMaker {
protected:
    User user;
public:
    static string getMapName(string &path);
    virtual void getRouteInfo() = 0;
    virtual void makeRoute() = 0;
    static vector<string> getAvailableMaps(string & path);
    static RouteMaker * openMap(string &mapDescription);
};

class SimpleRouteMaker : public RouteMaker {
private:
    MapContainer<VertexInfoTXT> * mapContainer = nullptr;
public:
    SimpleRouteMaker(string map);
    void getRouteInfo() override;
    void makeRoute() override;
    ~SimpleRouteMaker();
};

class OSMRouteMaker : public RouteMaker {
private:
    MapContainer<VertexInfoXML> * mapContainer = nullptr;
public:
    OSMRouteMaker(string map);
    void getRouteInfo() override;
    void makeRoute() override;
    ~OSMRouteMaker();
};



#endif //SRC_ROUTEMAKER_H
