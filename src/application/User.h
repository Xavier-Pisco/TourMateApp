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
};


#endif //SRC_USER_H
