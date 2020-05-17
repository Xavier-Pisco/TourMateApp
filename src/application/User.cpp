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

void User::setOrigin(Vertex<VertexInfoXML, Road> *origin) {
    this->origin=origin;
}

void User::setDestination(Vertex<VertexInfoXML, Road> *destination) {
    this->destination=destination;
}
