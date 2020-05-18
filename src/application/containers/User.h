#ifndef SRC_USER_H
#define SRC_USER_H

#include <string>
#include <iostream>
#include <list>
#include "GraphClasses.h"
#include "../graph/Graph.h"

using namespace std;


class User {
private:
    float time;
    Vertex<VertexInfoXML,WayInfoXML> * origin;
    Vertex<VertexInfoXML,WayInfoXML> *destination;
    list<string> *preferenceList;
    list<POI> *poiList;
public:
    User();
    void setOrigin(Vertex<VertexInfoXML,WayInfoXML> * origin);
    void setDestination(Vertex<VertexInfoXML,WayInfoXML> * destination);
    void setAvailability(float time);

    float getTime() const;

    Vertex<VertexInfoXML, WayInfoXML> *getOrigin() const;

    Vertex<VertexInfoXML, WayInfoXML> *getDestination() const;

    list<string> *getPreferenceList() const;

    list<POI> *getPoiList() const;

    void addPreference(string str);
};


#endif //SRC_USER_H
