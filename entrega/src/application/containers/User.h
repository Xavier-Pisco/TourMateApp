#ifndef SRC_USER_H
#define SRC_USER_H

#include <string>
#include <iostream>
#include <list>
#include "Route.h"

using namespace std;


template<class T>
class User {
public:
    enum routeMode {FOOT, CAR};
private:
    int time = -1; // in minutes, -1 if not to visit POIs
    Vertex<T> * origin, * destination;
    vector<string> preferenceList;
    Route<T> * route;
    routeMode routeMode;
public:
    User();
    void setOrigin(Vertex<T> * origin);
    
    void setDestination(Vertex<T> * destination);

    void setAvailability(int time);

    void setRoute(Route<T> * r);

    void setRouteMode(enum routeMode r);

    float getTime() const;

    Vertex<T> *getOrigin() const;

    Vertex<T> *getDestination() const;

    Route<T> *getRoute() const;

    enum routeMode getRouteMode() const;

    const vector<string> &getPreferenceList() const;

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
void User<T>::setRouteMode(enum routeMode r) {
    routeMode = r;
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
const vector<string> &User<T>::getPreferenceList() const {
    return preferenceList;
}

template<class T>
void User<T>::addPreference(string str) {
    this->preferenceList.push_back(str);
}

template<class T>
void User<T>::setRoute(Route<T> * r) {
    route = r;
}

template<class T>
Route<T> * User<T>::getRoute() const {
    return route;
}

template<class T>
enum User<T>::routeMode User<T>::getRouteMode() const {
    return routeMode;
}


#endif //SRC_USER_H
