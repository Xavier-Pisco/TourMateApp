#include "RouteMaker.h"



SimpleRouteMaker::SimpleRouteMaker(string map) {
    mapContainer = new SimpleMapContainer(map);

    mapContainer->getGraphViewer()->viewGraph();
}

void SimpleRouteMaker::getRouteInfo() {

}

void SimpleRouteMaker::makeRoute() {

}


SimpleRouteMaker::~SimpleRouteMaker() {
    delete mapContainer;
}
