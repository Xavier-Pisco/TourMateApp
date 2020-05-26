#include "RouteMaker.h"

#include <utility>


SimpleRouteMaker::SimpleRouteMaker(string map) {
    mapContainer = new SimpleMapContainer(map);
    graphAnalyzer = new GraphAnalyzer<VertexInfoTXT>(mapContainer->getGraph());

    mapContainer->getGraphViewer()->viewGraph();
}


void SimpleRouteMaker::getRouteInfo() {
    Drawer::drawTitle("Route info");
    cout << "Insert 'stop' at any time to cancel." << endl;

    Vertex<VertexInfoTXT> * vx;


    Drawer::drawTitle("Route Mode", 0, 40, true, "left"); cout << endl;

    Menu m;
    m.addOption("cancel", 10);
    m.addOption("on foot", User<VertexInfoTXT>::FOOT);
    m.addOption("by car", User<VertexInfoTXT>::CAR);

    m.draw("");
    unsigned opt = m.getResponse("Choose an option from the menu:");

    if (opt == 10) throw CancelInput();
    user.setRouteMode((enum User<VertexInfoTXT>::routeMode) opt);

    cout << endl;
    Drawer::drawTitle("Origin location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer, false);
    user.setOrigin(vx);

    cout << endl; Drawer::drawTitle("Destination location", 0, 40, true, "left"); cout << endl;

    vx = UserInput::getVertex(mapContainer, false, false);
    user.setDestination(vx);

    bool visitPOIs;
    if (vx != nullptr) {
        visitPOIs = UserInput::getConfirmation("Do you wish to visit POIs if you have time?");
    } else visitPOIs = true;
    if (visitPOIs) {
        cout << endl;
        int time = UserInput::getInt("Available time in minutes: ", 1);
        user.setAvailability(time);

        cout << endl;
        Drawer::drawTitle("Preferences", 0, 40, true, "left");
        cout << endl;

        Menu menu;
        vector<string> availableCategories = mapContainer->getAvailableCategories();
        menu.addExtraInput(availableCategories);

        menu.addOption("cancel");
        menu.addOption("done");

        bool done = false;
        while (!done) {
            cout << endl;
            menu.draw("");

            string res;
            int opt = menu.getResponse("Choose an option", res);

            switch (opt) {
                case -1:
                    user.addPreference(res);
                    break;
                case 0:
                    throw CancelInput();
                case 1:
                    done = true;
                default:
                    break;
            }
        }
    }

    if (makeRoute() != -1) displayRoute();
}

int SimpleRouteMaker::makeRoute() {
    vector<string> userPreferences = user.getPreferenceList();
    vector<Vertex<VertexInfoTXT>*> pOIVertexes = mapContainer->getPOIVertexes(), toErase;

    pOIVertexesPreference.erase(pOIVertexesPreference.begin(), pOIVertexesPreference.end());

    for (auto it = pOIVertexes.begin(); it != pOIVertexes.end(); it++) {
        if (find(userPreferences.begin(), userPreferences.end(), (*it)->getInfo().getCategory()) != userPreferences.end() && (*it) != user.getDestination() && (*it) != user.getOrigin()) {
            pOIVertexesPreference.push_back(*it);
        }
    }

    // removing POIs that don't have a path to the destination
    if (mapContainer->getNotConnected() && user.getDestination() != nullptr) { // if the graph is not strongly connected
        for (auto v : pOIVertexesPreference) {
            vector<Vertex<VertexInfoTXT> *> vc = mapContainer->getGraph()->bfs(v);

            if (find(vc.begin(), vc.end(), user.getDestination()) == vc.end()) {
                toErase.push_back(v);
            }
        }
    }

    for (auto v : toErase) pOIVertexesPreference.erase(find(pOIVertexesPreference.begin(), pOIVertexesPreference.end(), v));


    try {
        if (user.getTime() == -1) {  // simple GPS
            GPSRoute();
            return 0;
        } else if (user.getDestination() == user.getOrigin()) {  // destination = origin
            return returnToOriginRoute();
        } else if (user.getDestination() != nullptr) {  // with destination
            return fillExtraTimeRoute();
        } else {  // no destination
            return touristicRoute();
        }
    } catch (ImpossibleToReach &c) {
        cout << "Impossible to reach! Sorry, it isn't possible to make the route you want." << endl;
        return -1;
    }
}

int SimpleRouteMaker::fillExtraTimeRoute() {
    vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>> route;
    Vertex<VertexInfoTXT> * origin = user.getOrigin(), * destination = user.getDestination();

    double estimatedTime = 0;
    double routeDist = 0;
    int poiCount = 0;

    Vertex<VertexInfoTXT> * currVx = origin;
    mapContainer->getGraph()->dijkstra(currVx);

    if (destination->getDist() == DBL_MAX) {
        throw ImpossibleToReach();
    } else if (calculateTimeFromDistance(destination->getDist()) > user.getTime()) {
        pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = mapContainer->getGraph()->getPathToFromDijkstra(origin, destination);
        routeDist = p.second;
        route.insert(route.end(), p.first.begin(), p.first.end());
        cout << "The route to your destination takes longer than the time you have available!" << endl
            << "We've found the most efficient path from the origin to the destination" << endl << endl;
    } else {
        while (currVx != destination) {
            pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = getNextPathPart(currVx, destination, estimatedTime, false);

            route.insert(route.end(), p.first.begin(), p.first.end());

            estimatedTime += calculateTimeFromDistance(p.second);
            routeDist += p.second;


            poiCount++;

            route.empty() ? currVx = destination : currVx = route.at(route.size()-1).first;
        }
    }

    route.insert(route.begin(), pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>(origin, NULL)); // inserts the start vertex, which isn't inserted in the loop
    user.setRoute(new Route<VertexInfoTXT>(route, routeDist));
    return poiCount;
}

double SimpleRouteMaker::calculateVectorAngle(pair<double, double> &v1, pair<double, double> &v2) {
    double mod1 = sqrt(pow(v1.first, 2) + pow(v1.second, 2));
    double mod2 = sqrt(pow(v2.first, 2) + pow(v2.second, 2));

    double scalar = (v1.first * v2.first) +
                    (v1.second * v2.second);
    double angleCos = scalar / (mod1 * mod2);

    return acos(angleCos);
}

pair<double, double> SimpleRouteMaker::calculateVectorFrom2Vx(Vertex<VertexInfoTXT> * vx1, Vertex<VertexInfoTXT> * vx2) {
    pair<double, double> v;
    v.first = vx2->getInfo().getLat() - vx1->getInfo().getLat();
    v.second = vx2->getInfo().getLon() - vx1->getInfo().getLon();
    return v;
}

Vertex<VertexInfoTXT> * SimpleRouteMaker::getCandidate(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination, double currentTime, bool ignoreAngle) {

    for (auto c : pOIVertexesPreference) {
        c->setLessPreferable(false);
        if (!ignoreAngle || currentTime > user.getTime()/2 ) {

            pair<double, double> vectorFromCurrVxToC = calculateVectorFrom2Vx(currVx, c);
            pair<double, double> vectorFromCurrVxToDestination = calculateVectorFrom2Vx(currVx, destination);

            double angle = calculateVectorAngle(vectorFromCurrVxToC, vectorFromCurrVxToDestination);

            if (angle >= M_PI / 2 || angle <= -M_PI / 2) { c->setLessPreferable(true); }
        }
    }

    sort(pOIVertexesPreference.begin(), pOIVertexesPreference.end(), [](Vertex<VertexInfoTXT>* v1, Vertex<VertexInfoTXT>* v2) {
        if (v1->getLessPreferable() == !v2->getLessPreferable()) {
            return !v1->getLessPreferable();
        }
        return v1->getDist() < v2->getDist();
    });

    return pOIVertexesPreference.empty() ? destination : (pOIVertexesPreference.at(0)->getDist() == DBL_MAX ? destination : pOIVertexesPreference.at(0));
}


pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double>  SimpleRouteMaker::getNextPathPart(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination, double currTime, bool returnToOrigin) {
    if (currVx->getDist() != 0) mapContainer->getGraph()->dijkstra(currVx);

    // greedy
    Vertex<VertexInfoTXT> * candidate = getCandidate(currVx, destination, currTime, returnToOrigin); // find candidate

    double candidateTime = currTime + calculateTimeFromDistance(candidate->getDist()); // the time to get from the origin to the candidate

    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> candidatePath = mapContainer->getGraph()->getPathToFromDijkstra(currVx, candidate);
    if (candidate == destination) return candidatePath; // == destinationPath
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> destinationPath = mapContainer->getGraph()->getPathToFromDijkstra(currVx, destination);

    mapContainer->getGraph()->dijkstra(candidate);

    // backtracking
    if (candidateTime + calculateTimeFromDistance(destination->getDist()) < user.getTime()) { // if finds an optimal solution
        pOIVertexesPreference.erase(find(pOIVertexesPreference.begin(), pOIVertexesPreference.end(), candidate));
        return candidatePath;
    }
    return destinationPath;
}

int SimpleRouteMaker::returnToOriginRoute() {
    vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>> route;
    Vertex<VertexInfoTXT> * origin = user.getOrigin();

    double estimatedTime = 0;
    double routeDist = 0;
    int poiCount = 0;

    Vertex<VertexInfoTXT> * currVx = origin;
    mapContainer->getGraph()->dijkstra(currVx);
    bool starting = true;

    while (starting || currVx != origin) {
        pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = getNextPathPart(currVx, origin, estimatedTime, true);

        route.insert(route.end(), p.first.begin(), p.first.end());

        estimatedTime += calculateTimeFromDistance(p.second);
        routeDist += p.second;

        poiCount++;

        route.empty() ? currVx = origin : currVx = route.at(route.size()-1).first;
        if (currVx == origin && starting) cout << "You don't have time to visit any POIs!" << endl << endl;
        starting = false;
    }

    route.insert(route.begin(), pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>(origin, NULL)); // inserts the start vertex, which isn't inserted in the loop
    user.setRoute(new Route<VertexInfoTXT>(route, routeDist));
    return poiCount;
}

int SimpleRouteMaker::touristicRoute() {
    vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>> route;
    Vertex<VertexInfoTXT> * origin = user.getOrigin();

    double estimatedTime = 0;
    double routeDist = 0;
    int poiCount = 0;

    Vertex<VertexInfoTXT> * currVx = origin;

    while (true) {
        pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = getNextPathPartTouristic(currVx, estimatedTime);
        if (p.first.empty()) break;

        route.insert(route.end(), p.first.begin(), p.first.end());

        estimatedTime += calculateTimeFromDistance(p.second);
        routeDist += p.second;

        poiCount++;

        currVx = route.at(route.size()-1).first;
    }

    route.insert(route.begin(), pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>(origin, NULL)); // inserts the start vertex, which isn't inserted in the loop
    user.setRoute(new Route<VertexInfoTXT>(route, routeDist));
    return poiCount;
}

pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double>  SimpleRouteMaker::getNextPathPartTouristic(Vertex<VertexInfoTXT> * currVx, double currTime) {
    mapContainer->getGraph()->dijkstra(currVx);
    // greedy
    Vertex<VertexInfoTXT> * candidate = getCandidate(currVx, currVx, -1, true); // find candidate
    if (candidate == currVx) return {};

    double candidateTime = currTime + calculateTimeFromDistance(candidate->getDist()); // the time to get from the origin to the candidate

    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> candidatePath = mapContainer->getGraph()->getPathToFromDijkstra(currVx, candidate);

    if (candidateTime < user.getTime()) { // if finds an optimal solution
        pOIVertexesPreference.erase(find(pOIVertexesPreference.begin(), pOIVertexesPreference.end(), candidate));
        return candidatePath;
    }
    return {};
}

double SimpleRouteMaker::calculateTimeFromDistance(double dist /*in km*/) { // returns in minutes
    if (user.getRouteMode() == User<VertexInfoTXT>::CAR) {  // CAR
        // considering avg speed = 9 m/s
        return abs((dist * 1000 / 9.) / 60.);
    } else {  // FOOT
        // avg speed = 1.4m/s = 5km/h
        return abs((dist * 1000 / 1.4) / 60.);
    }
}

void SimpleRouteMaker::GPSRoute() {
    mapContainer->getGraph()->dijkstra(user.getOrigin());
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination());


    p.first.insert(p.first.begin(), pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>(user.getOrigin(), NULL)); // inserts the start vertex, which isn't inserted in the loop

    user.setRoute(new Route<VertexInfoTXT>(p.first, p.second));
}

void SimpleRouteMaker::displayRoute() {

    Drawer::drawTitle("The Route");

    cout << user.getRoute()->getStringRepresentation();

    cout << endl << endl << "Distance = " << user.getRoute()->getDist() << "km" << endl << "It will take approximately " << calculateTimeFromDistance(user.getRoute()->getDist()) << " mins" << endl << endl;

    cout << "If you see any POIs you didn't mean to visit, it is because they were in the path anyway" << endl << endl;

    bool conf = UserInput::getConfirmation("Do you want to see the route in the graph viewer? \nIf you say yes, and you've closed the graph viewer window, the program will freeze.");
    if (conf) mapContainer->getGraphViewer()->viewRoute(user.getRoute());
}

void SimpleRouteMaker::openGraphAnalyzer() {
    graphAnalyzer->menu();
}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}

void SimpleRouteMaker::setMapContainer(SimpleMapContainer *smc) {
    mapContainer = smc;
}

double SimpleRouteMaker::getRouteDist() {
    return user.getRoute()->getDist();
}

void SimpleRouteMaker::setUser(User<VertexInfoTXT> u) {
    this->user = u;
}
