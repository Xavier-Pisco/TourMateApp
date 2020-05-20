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



#endif //SRC_ROUTEMAKER_H
