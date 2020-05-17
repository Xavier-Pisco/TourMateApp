#ifndef SRC_ROUTEMAKER_H
#define SRC_ROUTEMAKER_H

#include <string>
#include <vector>
#include "../graph/GraphClasses.h"
#include "../graph/algorithms/dijkstra.h"
#include "input/Converter.h"
#include "Menu.h"
#include "Drawer.h"
#include <dirent.h>
#include "User.h"

using namespace std;

class RouteMaker {
private:
    Graph<VertexInfoXML, Road> * graph = nullptr;
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
