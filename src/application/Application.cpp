#include <iostream>
#include <cmath>
#include "Application.h"
#include "ui/Menu.h"
#include "logic/RouteMaker.h"
#include "ui/Drawer.h"


void Application::start() {
    while( mainMenu() != -1 );
    exit();
}

int Application::mainMenu() {
    Menu menu;

    menu.addOption("exit application");
    menu.addOption("start a route with a OSM map");
    menu.addOption("start a route with a simple txt map");
    menu.addOption("start a route with a grid graph");
    menu.addOption("view available maps");


    Drawer::drawTitle("TourMateApp");
    cout << endl;
    menu.draw();
    cout << endl << endl;
    unsigned opt = menu.getResponse("Choose an option from the menu:");

    OSMRouteMaker osmRouteMaker;
    SimpleRouteMaker simpleRouteMaker;
    string path;

    switch(opt) {
        case 0:
            return -1;
        case 1:
            path = "../maps/osm/";
            osmRouteMaker.start(path);
            break;
        case 2:
            path = "../maps/simple/";
            simpleRouteMaker.start(path);
            break;
        case 3:
            break;
        case 4:
        default:
            break;
    }
    return 0;
}



int Application::latToY(const string &lat, int height, pair<double, double> &minCoords, pair<double, double> &maxCoords) {
    double latD = strtod(lat.c_str(), nullptr);
    double r = ((latD - minCoords.first)/(maxCoords.first - minCoords.first))*height;
    return (int) round(r);
}

int Application::lonToX(const string &lon, int width, pair<double, double> &minCoords, pair<double, double> &maxCoords) {
    double lonD = strtod(lon.c_str(), nullptr);
    double r = ((lonD - minCoords.second)/(maxCoords.second - minCoords.second))*width;
    return (int) round(r);
}

void Application::exit() {
    // do stuff
}
