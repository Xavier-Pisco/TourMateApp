#include "RouteMaker.h"



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
    // TODO add option for "return to starting point"
    vx = UserInput::getVertex(mapContainer, false, false);
    user.setDestination(vx);

    bool visitPOIs;
    if (vx != nullptr) {
        visitPOIs = UserInput::getConfirmation("Do you wish to visit POIs if you have time?");
    } else visitPOIs = true;
    if (visitPOIs) {
        cout << endl;
        int time = UserInput::getInt("Available time in minutes: ", 0);
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

    makeRoute();
}

void SimpleRouteMaker::makeRoute() {
    for (auto v : mapContainer->getGraph()->getVertexSet()) {
        v->setVisited(false);
    }

    if (user.getTime() == -1) {  // simple GPS
        GPSRoute();
    } else if (user.getDestination() == user.getOrigin()) {  // destination = origin
        returnToOriginRoute();
    } else if (user.getDestination() != nullptr) {  // with destination
        fillExtraTimeRoute();
    } else {  // no destination
        touristicRoute();
    }

    displayRoute();
}

void SimpleRouteMaker::fillExtraTimeRoute() {
    vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>> route;
    Vertex<VertexInfoTXT> * origin = user.getOrigin(), * destination = user.getDestination();

    unsigned estimatedTime = 0;
    double routeDist = 0;

    Vertex<VertexInfoTXT> * currVx = origin;
    mapContainer->getGraph()->dijkstra(currVx);
    while (currVx != destination) {
        pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = getNextPathPart(currVx, destination, estimatedTime);

        route.insert(route.end(), p.first.begin(), p.first.end());
        estimatedTime += calculateTimeFromDistance(p.second);
        routeDist += p.second;
        currVx = route.at(route.size()-1).first;
    }

    route.insert(route.begin(), pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>(origin, NULL)); // inserts the start vertex, which isn't inserted in the loop
    user.setRoute(new Route<VertexInfoTXT>(route, routeDist));
}

Vertex<VertexInfoTXT> * SimpleRouteMaker::getCandidate(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination) {
    vector<Vertex<VertexInfoTXT>*> pOIVertexes = mapContainer->getPOIVertexes();
    vector<string> userPreferences = user.getPreferenceList();

    vector<Vertex<VertexInfoTXT>*> toErase;
    for (auto it = pOIVertexes.begin(); it != pOIVertexes.end(); it++) {
        if ((*it)->getVisited() || find(userPreferences.begin(), userPreferences.end(), (*it)->getInfo().getCategory()) == userPreferences.end()) {
            toErase.push_back(*it);
        } else {
            Vertex<VertexInfoTXT> * c = *it;
            pair<double, double> vectorFromCurrVxToC;
            vectorFromCurrVxToC.first = c->getInfo().getLat()-currVx->getInfo().getLat();
            vectorFromCurrVxToC.second = c->getInfo().getLon()-currVx->getInfo().getLon();

            pair<double, double> vectorFromCurrVxToDestination;
            vectorFromCurrVxToDestination.first = destination->getInfo().getLat()-currVx->getInfo().getLat();
            vectorFromCurrVxToDestination.second = destination->getInfo().getLon()-currVx->getInfo().getLon();

            double mod1 = sqrt(pow(vectorFromCurrVxToC.first, 2) + pow(vectorFromCurrVxToC.second, 2));
            double mod2 = sqrt(pow(vectorFromCurrVxToDestination.first, 2) + pow(vectorFromCurrVxToDestination.second, 2));

            double scalar = (vectorFromCurrVxToC.first * vectorFromCurrVxToDestination.first) + (vectorFromCurrVxToC.second * vectorFromCurrVxToDestination.second);
            double angleCos = scalar/(mod1*mod2);

            double angle = acos(angleCos);
            if (angle >= M_PI/2 || angle <= -M_PI/2) toErase.push_back(*it);
        }
    }
    for (auto it : toErase) pOIVertexes.erase(find(pOIVertexes.begin(), pOIVertexes.end(), it));

    sort(pOIVertexes.begin(), pOIVertexes.end(), [](Vertex<VertexInfoTXT>* v1, Vertex<VertexInfoTXT>* v2) {
        return v1->getDist() < v2->getDist();
    });
    return pOIVertexes.empty() ? destination : pOIVertexes.at(0);
}

pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double>  SimpleRouteMaker::getNextPathPart(Vertex<VertexInfoTXT> * currVx, Vertex<VertexInfoTXT> * destination, unsigned currTime) {
    if (currVx->getDist() != 0) {
        mapContainer->getGraph()->dijkstra(currVx);
    }

    Vertex<VertexInfoTXT> * candidate = getCandidate(currVx, destination); // find candidate

    unsigned candidateTime = currTime + calculateTimeFromDistance(candidate->getDist());
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> candidatePath = mapContainer->getGraph()->getPathToFromDijkstra(currVx, candidate);
    if (candidate == destination) return candidatePath;
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> destinationPath = mapContainer->getGraph()->getPathToFromDijkstra(currVx, destination);

    mapContainer->getGraph()->dijkstra(candidate);

    if (candidateTime + calculateTimeFromDistance(destination->getDist()) < user.getTime()) { // if finds an optimal solution
        candidate->setVisited(true);
        return candidatePath;
    }
    return destinationPath;
}

void SimpleRouteMaker::returnToOriginRoute() {

}

void SimpleRouteMaker::touristicRoute() {

}

unsigned SimpleRouteMaker::calculateTimeFromDistance(double dist /*in km*/) { // returns in minutes
    if (user.getRouteMode() == User<VertexInfoTXT>::CAR) {  // CAR
        // considering avg speed = 50 km/h = 13.9 m/s
        return (unsigned) abs((dist * 1000 / 13.9) / 60);
    } else {  // FOOT
        // avg speed = 1.4m/s = 5km/h
        return (unsigned) abs((dist * 1000 / 1.4) / 60);
    }
}

void SimpleRouteMaker::GPSRoute() {
    mapContainer->getGraph()->dijkstra(user.getOrigin());
    pair<vector<pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>>, double> p = mapContainer->getGraph()->getPathToFromDijkstra(user.getOrigin(), user.getDestination());

    p.first.insert(p.first.begin(), pair<Vertex<VertexInfoTXT>*, Edge<VertexInfoTXT>*>(user.getOrigin(), NULL)); // inserts the start vertex, which isn't inserted in the loop

    user.setRoute(new Route<VertexInfoTXT>(p.first, p.second));
}

void SimpleRouteMaker::displayRoute() {
    cout << endl << "Distance = " << user.getRoute()->getDist() << "km" << endl << "It will take approximately " << calculateTimeFromDistance(user.getRoute()->getDist()) << " mins" << endl << endl;

    cout << user.getRoute()->getStringRepresentation();

    bool conf = UserInput::getConfirmation("Do you want to see the route in the graph viewer? \nIf you say yes, and you've closed the graph viewer window, the program will freeze.");
    if (conf) mapContainer->getGraphViewer()->viewRoute(user.getRoute());
}

void SimpleRouteMaker::openGraphAnalyzer() {
    graphAnalyzer->menu();
}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
