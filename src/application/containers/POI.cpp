#include "POI.h"

POI::POI(double lat, double lon, string n, string d, string type, int avg) {
    coords.first = lat;
    coords.second = lon;
    name = n;
    description = d;
    averageSpentTime = avg;
    this->type = type;
}

const pair<double, double> &POI::getCoords() const {
    return coords;
}

const string &POI::getName() const {
    return name;
}

const string &POI::getDescription() const {
    return description;
}

int POI::getAverageSpentTime() const {
    return averageSpentTime;
}

const string &POI::getType() const {
    return type;
}

