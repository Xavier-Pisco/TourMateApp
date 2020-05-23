#ifndef SRC_USER_H
#define SRC_USER_H

#include <string>
#include <iostream>
#include <list>
#include "GraphClasses.h"
#include "../graph/Graph.h"

using namespace std;


template<class T>
class User {
private:
    int time; // in minutes
    Vertex<T> * origin;
    Vertex<T> *destination;
    vector<string> preferenceList;
    vector<POI> poiList;
public:
    User();
    void setOrigin(Vertex<T> * origin);
    void setDestination(Vertex<T> * destination);
    void setAvailability(int time);

    float getTime() const;

    Vertex<T> *getOrigin() const;

    Vertex<T> *getDestination() const;

    vector<string> getPreferenceList() const;

    vector<POI> getPoiList() const;

    void addPreference(string str);
};

template<class T>
User<T>::User() {
    this->origin = nullptr;
    this->destination = nullptr;
    time = -1;
}

template<class T>
void User<T>::setAvailability(int time) {
    this->time=time;
}

template<class T>
void User<T>::setOrigin(Vertex<T> *origin) {
    this->origin=origin;
}

template<class T>
void User<T>::setDestination(Vertex<T> *destination) {
    this->destination=destination;
}

template<class T>
float User<T>::getTime() const {
    return time;
}

template<class T>
Vertex<T> *User<T>::getOrigin() const {
    return origin;
}

template<class T>
Vertex<T> *User<T>::getDestination() const {
    return destination;
}

template<class T>
vector<string> User<T>::getPreferenceList() const {
    return preferenceList;
}

template<class T>
vector<POI> User<T>::getPoiList() const {
    return poiList;
}

template<class T>
void User<T>::addPreference(string str) {
    this->preferenceList.push_back(str);
}


#endif //SRC_USER_H
