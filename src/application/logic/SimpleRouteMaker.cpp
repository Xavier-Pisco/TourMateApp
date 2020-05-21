#include "RouteMaker.h"



SimpleRouteMaker::SimpleRouteMaker(string map) {
    mapContainer = new SimpleMapContainer(map);

    mapContainer->getGraphViewer()->viewGraph();

    UserInput::getLine("Press ENTER to close graph. Note: If you close on the 'x' of the window it will shutdown the program");
    mapContainer->getGraphViewer()->closeView();
}

void SimpleRouteMaker::getRouteInfo() {

}

void SimpleRouteMaker::makeRoute() {

}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
