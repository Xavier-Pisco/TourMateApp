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
private:
    MapContainer * mapContainer = nullptr;
    User user;
public:
    ~RouteMaker();
    void start();
    void openMap(string &map);
    void getRouteInfo();
    void makeRoute();
    vector<string> getAvailableMaps(string & path) const;
};


#endif //SRC_ROUTEMAKER_H
