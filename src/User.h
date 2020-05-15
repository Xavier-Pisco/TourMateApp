#ifndef SRC_USER_H
#define SRC_USER_H

#include <string>
#include <iostream>
#include <list>
#include "graph/GraphClasses.h"
#include "graph/Graph.h"

using namespace std;


class User {
private:
    //dados de entrada
    float disp;
    Vertex<RoadIntersection,Road> * origin;
    Vertex<RoadIntersection,Road> *destiny;
    list<string> *preferenceList;
    list<POI> *poiList;
public:
    User();
    void findOrigem(string str);
    void findDestino(string str);
    void setDisponibilidade(float time);
};


#endif //SRC_USER_H
