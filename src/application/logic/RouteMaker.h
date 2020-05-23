#ifndef SRC_ROUTEMAKER_H
#define SRC_ROUTEMAKER_H

#include <string>
#include <vector>
#include <dirent.h>

#include "../containers/Route.h"
#include "../graph/algorithms/dijkstra.h"
#include "../ui/Menu.h"
#include "../ui/Drawer.h"
#include "../containers/User.h"
#include "../input/UserInput.h"
#include "GraphAnalyzer.h"

using namespace std;

class RouteMaker {
protected:
    User user;
public:
    static string getMapName(string &path);
    virtual void getRouteInfo() = 0;
    virtual void makeRoute() = 0;
    virtual void openGraphAnalyzer() = 0;
    static vector<string> getAvailableMaps(string & path);
    static RouteMaker * openMap(string &mapDescription);
    virtual ~RouteMaker() = default;
};

class SimpleRouteMaker : public RouteMaker {
private:
    SimpleMapContainer * mapContainer;
    GraphAnalyzer<VertexInfoTXT> * graphAnalyzer;
public:
    explicit SimpleRouteMaker(string map);
    void getRouteInfo() override;
    void openGraphAnalyzer() override;
    void makeRoute() {};
    void makeRoute(Vertex<VertexInfoTXT> * v1, Vertex<VertexInfoTXT> *v2);
    ~SimpleRouteMaker() override;
};

class OSMRouteMaker : public RouteMaker {
private:
    OSMapContainer * mapContainer;
    GraphAnalyzer<VertexInfoXML> * graphAnalyzer;
public:
    explicit OSMRouteMaker(string map);
    void getRouteInfo() override;
    void openGraphAnalyzer() override;
    void makeRoute() override;
    ~OSMRouteMaker() override;
};



#endif //SRC_ROUTEMAKER_H
