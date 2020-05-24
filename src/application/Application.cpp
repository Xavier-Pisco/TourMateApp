#include <iostream>
#include <cmath>
#include "Application.h"
#include "logic/PerformanceTests.h"


int Application::mainMenu() {
    Menu menu;
    string mapDescription;
    RouteMaker * routeMaker = nullptr;

    menu.addOption("exit application");

    menu.addOption("Open Map");
    menu.addOption("Start a Route");
    menu.addOption("Graph Connectivity Checks");
    menu.addOption("Performance Tests");


    while(true) {
        Drawer::drawTitle("TourMateApp");

        if (routeMaker == nullptr) cout << endl << "You haven't opened a map yet." << endl;
        else cout << endl << "You have a map opened: " << mapDescription << endl;

        cout << endl;
        menu.draw();
        cout << endl << endl;
        unsigned opt = menu.getResponse("Choose an option from the menu:");
        string path;

        try {
            switch (opt) {
                case 0:
                    delete routeMaker;
                    return -1;
                case 1:
                    delete routeMaker;
                    routeMaker = RouteMaker::openMap(mapDescription);
                    break;
                case 2:
                    if (routeMaker != nullptr) routeMaker->getRouteInfo();
                    else UserInput::waiter("You haven't opened a map yet.");
                    break;
                case 3:
                    if (routeMaker != nullptr) routeMaker->openGraphAnalyzer();
                    else UserInput::waiter("You haven't opened a map yet.");
                    break;
                case 4:
                    PerformanceTests::menu();
                default:
                    break;
            }
        } catch (CancelInput &c) { cout << "You cancelled the operation." << endl; }
    }
}


int Application::latToY(double lat, int height, pair<double, double> &minCoords, pair<double, double> &maxCoords) {
    double r = ((lat - minCoords.first)/(maxCoords.first - minCoords.first))*height;
    return (int) round(r);
}

int Application::lonToX(double lon, int width, pair<double, double> &minCoords, pair<double, double> &maxCoords) {
    double r = ((lon - minCoords.second)/(maxCoords.second - minCoords.second))*width;
    return (int) round(r);
}
