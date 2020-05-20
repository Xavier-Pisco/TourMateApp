#include "User.h"

User::User() {
    this->origin=NULL;
    this->destination=NULL;
    this->preferenceList=NULL;
    this->poiList=NULL;
}

void User::setAvailability(float time) {
    this->time=time;
}

void User::setOrigin(Vertex<VertexInfoXML> *origin) {
    this->origin=origin;
}

void User::setDestination(Vertex<VertexInfoXML> *destination) {
    this->destination=destination;
}

float User::getTime() const {
    return time;
}

Vertex<VertexInfoXML> *User::getOrigin() const {
    return origin;
}

Vertex<VertexInfoXML> *User::getDestination() const {
    return destination;
}

list<string> *User::getPreferenceList() const {
    return preferenceList;
}

list<POI> *User::getPoiList() const {
    return poiList;
}

void User::addPreference(string str) {
    this->preferenceList->push_back(str);
}