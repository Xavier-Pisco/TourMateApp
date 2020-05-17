#ifndef SRC_ROUTEMAKER_H
#define SRC_ROUTEMAKER_H

#include <string>
#include <vector>
#include "../containers/GraphClasses.h"
#include "../graph/algorithms/dijkstra.h"
#include "../input/Converter.h"
#include "../ui/Menu.h"
#include "../ui/Drawer.h"
#include <dirent.h>
#include "../containers/User.h"
#include "../ui/GraphViewerCustom.h"
#include "../input/UserInput.h"

using namespace std;

class RouteMaker {
private:
    Graph<VertexInfoXML, Road> * graph = nullptr;
    GraphViewerCustom * graphViewer = nullptr;
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
