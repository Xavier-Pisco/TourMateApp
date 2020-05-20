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
    Vertex<VertexInfoXML> * origin;
    Vertex<VertexInfoXML> *destination;
    list<string> *preferenceList;
    list<POI> *poiList;
public:
    User();
    void setOrigin(Vertex<VertexInfoXML> * origin);
    void setDestination(Vertex<VertexInfoXML> * destination);
    void setAvailability(float time);

    float getTime() const;

    Vertex<VertexInfoXML> *getOrigin() const;

    Vertex<VertexInfoXML> *getDestination() const;

    list<string> *getPreferenceList() const;

    list<POI> *getPoiList() const;

    void addPreference(string str);
};


#endif //SRC_USER_H
