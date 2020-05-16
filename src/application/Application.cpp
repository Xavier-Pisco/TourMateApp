#include <iostream>
#include <cmath>
#include "Application.h"
#include "Menu.h"
#include "RouteMaker.h"
#include "Drawer.h"


void Application::start() {
    //start user---
    /*UserInput userInput;
    User user;
    string origem, destino;

    origem=userInput.getLine("Ponto de Origem: ");
    user.findOrigem(origem);

    destino=userInput.getLine("Ponto de Chegada: ");
    user.findDestino(destino);

    float tempo=userInput.getFloat("Quanto tempo disponível: ");
    user.setDisponibilidade(tempo);
    //-----------*/


    while( mainMenu() != -1 );

    /*graph = Converter::getGraphFromOSMFile("../maps/centro_aliados.osm");
    if (mode == DEBUG) viewGraph();*/


    exit();
}

int Application::mainMenu() {
    Menu menu;

    menu.addOption("exit application");
    menu.addOption("start a route");
    menu.addOption("view available maps");


    Drawer::drawTitle("TourMateApp");
    cout << endl;
    menu.draw();
    cout << endl << endl;
    unsigned opt = menu.getResponse("Choose an option from the menu:");

    RouteMaker routeMaker;

    switch(opt) {
        case 0:
            return -1;
        case 1:
            routeMaker.start();
            break;
        case 2:
        default:
            break;
    }
    return 0;
}



int Application::latToY(const string &lat, int width, pair<double, double> &minCoords, pair<double, double> &maxCoords) {
    double latD = strtod(lat.c_str(), nullptr);
    double r = ((latD - minCoords.first)/(maxCoords.first - minCoords.first))*width;
    return (int) round(r);
}

int Application::lonToX(const string &lon, int height, pair<double, double> &minCoords, pair<double, double> &maxCoords) {
    double lonD = strtod(lon.c_str(), nullptr);
    double r = ((lonD - minCoords.second)/(maxCoords.second - minCoords.second))*height;
    return (int) round(r);
}

void Application::exit() {
    // do stuff
}
