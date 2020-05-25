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
    /**
     * @brief obtains the name of the map
     * @param path the path to the map file
     * @return the name
     */
    static string getMapName(string &path);

    /**
     * @brief gets the route info from the user
     */
    virtual void getRouteInfo() = 0;

    /**
     * @brief opens the graph analyzer
     */
    virtual void openGraphAnalyzer() = 0;

    /**
     * @brief gets the available maps in a folder
     * @param path the path to the folder
     * @return vector with the file names
     */
    static vector<string> getAvailableMaps(string & path);

    /**
     * @brief opens a map
     * @param mapDescription - returns the map description through this argument
     * @return pointer to the routemaker object created
     */
    static RouteMaker * openMap(string &mapDescription);

    /**
     * @brief destructor
     */
    virtual ~RouteMaker() = default;
};

class SimpleRouteMaker : public RouteMaker {
private:
    SimpleMapContainer * mapContainer = nullptr;
    GraphAnalyzer<VertexInfoTXT> * graphAnalyzer = nullptr;
    User<VertexInfoTXT> user;

    /* Auxiliary fields */
    vector<Vertex<VertexInfoTXT>*> pOIVertexesPreference;
public:
    SimpleRouteMaker() = default;
    /**
     * @brief the map to be opened
     * @param map the path to the map
     */
    explicit SimpleRouteMaker(string map);

    /**
     * @brief SET method for mapContainer
     * @param smc
     */
    void setMapContainer(SimpleMapContainer * smc);

    /**
     * @brief SET method for user
     * @param user
     */
    void setUser(User<VertexInfoTXT> user);

    /**
     * @brief displays the obtained route
     */
    void displayRoute();

    /**
     * @brief GET method for the route distance
     * @return the distance
     */
    double getRouteDist();

    /**
     * @brief calls the functions necessary to make the route based on user input from getRouteInfo()
     */
    void makeRoute();

    /**
     * @brief makes a simple route form origin to destination (simple dijkstra)
     */
    void GPSRoute();

    /**
     * @brief makes a purely touristic route, with no destination in mind
     */
    int touristicRoute();

    /**
     * @brief makes a route that return to the origin
     */
    int returnToOriginRoute();

    /**
     * @brief makes a route that gets to the destination in time but uses any extra time to visit POIs
     * @return the number of POIs visited
     */
    int fillExtraTimeRoute();

    /**
     * @brief auxiliar function, gets most adequate vertex for next position (greedy)
     * @param currVx - the current vertex
     * @param destination - the destination
     * @return pointer to the candidate
     */
    Vertex<VertexInfoTXT> * getCandidate(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination, double currentTime, bool ignoreAngle);

    /**
     * @brief calculates the angle bewteen v1 and v2
     * @param v1
     * @param v2
     * @return the angle
     */
    double calculateVectorAngle(pair<double, double> &v1, pair<double, double> &v2);

    /**
     * @brief calculates the vector from vx1 to vx2
     * @param vx1
     * @param vx2
     * @return the vector coords {lat, lon}
     */
    pair<double, double> calculateVectorFrom2Vx(Vertex<VertexInfoTXT> * vx1, Vertex<VertexInfoTXT> * vx2);

    /**
     * @brief auxiliar function for fillExtraTimeRoute and returnToOriginRoute, gets the next path part (from the current vertex to the next)
     * @param currVx - the current vertex
     * @param destination - the destination
     * @param currTime - current estimated time
     * @return {routePortion, dist}
     */
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double>  getNextPathPart(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination, double currTime, bool returnToOrigin);

    /**
     * @brief auxiliar function for touristicRoute, gets the next path part (from the current vertex to the next)
     * @param currVx - the current vertex
     * @param currTime - current estimated time
     * @return {routePortion, dist}
     */
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double>  getNextPathPartTouristic(Vertex<VertexInfoTXT> * currVx, double currTime);

    /**
     * @brief takes dist in km and returns estimated time of completion, based on user parameter (on foot, car, etc)
     * @param dist - distance in km
     * @return time in minutes
     */
    double calculateTimeFromDistance(double dist /*in km*/);

    void getRouteInfo() override;
    void openGraphAnalyzer() override;
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
    void displayRoute(Route<VertexInfoXML> * r);
    double calculateTimeFromDistance(double dist /*in km*/);
    ~OSMRouteMaker() override;
};



#endif //SRC_ROUTEMAKER_H
