#ifndef SRC_ROUTE_H
#define SRC_ROUTE_H

#include "GraphClasses.h"

template<class T>
class Route {
private:
    vector<pair<Vertex<T> *, Edge<T> *>> routePoints;
public:
    vector<Vertex<T>*> getVertexes() const {
        vector<Vertex<T>*> v;
        for (auto p : routePoints) {
            v.push_back(p.first);
        }
        return v;
    }

    void addRoutePoint(Vertex<T> * v, Edge<T> * e) {
        routePoints.push_back(pair<Vertex<T>*, Edge<T>*>(v, e));
    }

    void setRoutePoints(const vector<pair<Vertex<T>*, Edge<T>*>> & v) {
        routePoints = v;
    }
};


#endif //SRC_ROUTE_H
