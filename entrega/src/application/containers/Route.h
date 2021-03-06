#ifndef SRC_ROUTE_H
#define SRC_ROUTE_H

#include "../graph/GraphEdgeInfo.h"
#include "../ui/Drawer.h"

template<class T>
class Route {
private:
    vector<pair<Vertex<T> *, Edge<T> *>> routePoints;
    double dist;
public:
    /**
     * @brief constructor
     * @param v - the vector with the route's vertexes and edges
     * @param dist - the dist of the route
     */
    Route(vector<pair<Vertex<T> *, Edge<T> *>> v, double dist);

    /**
     * @brief returns only the vertexes of the route
     * @return vector with vertexes
     */
    vector<Vertex<T>*> getVertexes() const;

    /**
     * @brief GET method for dist
     * @return
     */
    double getDist() const;

    /**
     * @brief returns a text representation of the route
     * @return string
     */
    string getStringRepresentation(bool separated = false) const;
};


template<class T>
Route<T>::Route(vector<pair<Vertex<T> *, Edge<T> *>> v, double d) {
    this->routePoints = v;
    this->dist = d;
}


template<class T>
vector<Vertex<T>*> Route<T>::getVertexes() const {
    vector<Vertex<T>*> v;
    for (auto p : routePoints) {
        v.push_back(p.first);
    }
    return v;
}

template<class T>
string Route<T>::getStringRepresentation(bool separated) const {
    stringstream res;
    res << endl << "origin" << endl;
    int counter = 0;
    string lastName = "";
    for (pair<Vertex<T>*, Edge<T>*> p : routePoints) {
        bool foundStreetName = false;
        if (p.second != nullptr && p.second->hasInfoXML()) {
            string n = ((EdgeInfoXML<VertexInfoXML> *) p.second)->getInfo()->getXMLTagValue("name");
            if (n != "" and n != lastName) {
                res << endl << n << " -> ";
                foundStreetName = true;
                counter = 0;
            }
            lastName = n;
        }
        string c = p.first->getInfo().getCategory();
        if (c!= "") res << endl << "# POI: ";
        res << p.first->getInfo().getID();
        if (c != "") {
            res << ": " << c << endl;
            counter = 0;
        }
        if (separated && foundStreetName) res << endl;
        res << " -> ";
        counter ++;
        if (counter % 15 == 0) res << endl;
    }
    res << "destination" << endl << endl;
    return res.str();
}

template<class T>
double Route<T>::getDist() const {
    return dist;
}


#endif //SRC_ROUTE_H
