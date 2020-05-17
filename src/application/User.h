#ifndef SRC_USER_H
#define SRC_USER_H

#include <string>
#include <iostream>
#include <list>
#include "../graph/GraphClasses.h"
#include "../graph/Graph.h"

using namespace std;


class User {
private:
    float time;
    Vertex<VertexInfoXML,Road> * origin;
    Vertex<VertexInfoXML,Road> *destination;
    list<string> *preferenceList;
    list<POI> *poiList;
public:
    User();
    void setOrigin(Vertex<VertexInfoXML,Road> * origin);
    void setDestination(Vertex<VertexInfoXML,Road> * destination);
    void setAvailability(float time);

    float getTime() const;

    Vertex<VertexInfoXML, Road> *getOrigin() const;

    Vertex<VertexInfoXML, Road> *getDestination() const;

    list<string> *getPreferenceList() const;

    list<POI> *getPoiList() const;
};


#endif //SRC_USER_H
