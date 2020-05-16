#ifndef SRC_ROUTEMAKER_H
#define SRC_ROUTEMAKER_H

#include <string>
#include <vector>
#include "../graph/GraphClasses.h"
#include "../input/Converter.h"

using namespace std;

class RouteMaker {
private:
    Graph<VertexInfoXML, Road> * graph;
public:
    void start();
    void openMap(string &map);
    void getRouteInfo();
    vector<string> getAvailableMaps(string & path) const;
};


#endif //SRC_ROUTEMAKER_H
