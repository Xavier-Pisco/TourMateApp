#ifndef SRC_ROUTE_H
#define SRC_ROUTE_H

#include "../graph/GraphEdgeInfo.h"
#include "../ui/Drawer.h"


template<class T>
class Route {
private:
    vector<pair<Vertex<T> *, Edge<T> *>> routePoints;
public:
    /**
     * @brief constructor
     * @param v - the vector with the route's vertexes and edges
     */
    Route(vector<pair<Vertex<T> *, Edge<T> *>> &v);

    /**
     * @brief returns only the vertexes of the route
     * @return vector with vertexes
     */
    vector<Vertex<T>*> getVertexes() const;

    /**
     * @brief returns a text representation of the route
     * @return string
     */
    string getStringRepresentation() const;
};


template<class T>
Route<T>::Route(vector<pair<Vertex<T> *, Edge<T> *>> &v) {
    this->routePoints = v;
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
string Route<T>::getStringRepresentation() const {
    stringstream res;
    res << endl << "origin" << endl;
    int counter = 0;
    for (pair<Vertex<T>*, Edge<T>*> p : routePoints) {
        if (p.second != nullptr && p.second->hasInfoXML()) {
            string n = ((EdgeInfoXML<VertexInfoXML> *) p.second)->getInfo()->getXMLTagValue("name");
            if (n != "") res << n << " -> ";
        }
        string c = p.first->getInfo().getCategory();
        if (c!= "") res << endl << "# POI: ";
        res << p.first->getInfo().getID();
        if (c != "") {
            res << ": " << c << endl;
            counter = 0;
        }
        res << " -> ";
        counter ++;
        if (counter % 15 == 0) res << endl;
    }
    res << "destination" << endl << endl;
    return res.str();
}


#endif //SRC_ROUTE_H
