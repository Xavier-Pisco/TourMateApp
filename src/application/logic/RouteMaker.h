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
public:
    static string getMapName(string &path);
    virtual void getRouteInfo() = 0;
    virtual void openGraphAnalyzer() = 0;
    static vector<string> getAvailableMaps(string & path);
    static RouteMaker * openMap(string &mapDescription);
    virtual ~RouteMaker() = default;
};

class SimpleRouteMaker : public RouteMaker {
private:
    SimpleMapContainer * mapContainer;
    GraphAnalyzer<VertexInfoTXT> * graphAnalyzer;
    User<VertexInfoTXT> user;

    /* Auxiliary fields */
    vector<Vertex<VertexInfoTXT>*> pOIVertexesPreference;
public:
    SimpleRouteMaker() = default;
    explicit SimpleRouteMaker(string map);
    void setMapContainer(SimpleMapContainer * smc);
    void setUser(User<VertexInfoTXT> user);
    void displayRoute();
    void getRouteInfo() override;
    double getRouteDist();
    void openGraphAnalyzer() override;
    void makeRoute();
    void GPSRoute();
    void touristicRoute();
    void returnToOriginRoute();
    void fillExtraTimeRoute();
    Vertex<VertexInfoTXT> * getCandidate(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination);
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double>  getNextPathPart(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination, double currTime);
    double calculateTimeFromDistance(double dist /*in km*/);
    ~SimpleRouteMaker() override;
};

class OSMRouteMaker : public RouteMaker {
private:
    OSMapContainer * mapContainer;
    GraphAnalyzer<VertexInfoXML> * graphAnalyzer;
    User<VertexInfoXML> user;
public:
    explicit OSMRouteMaker(string map);
    void getRouteInfo() override;
    void openGraphAnalyzer() override;
    void makeRoute();
    ~OSMRouteMaker() override;
};



#endif //SRC_ROUTEMAKER_H
