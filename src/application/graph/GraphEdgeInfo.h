#ifndef SRC_GRAPHEDGEINFO_H
#define SRC_GRAPHEDGEINFO_H

#include "../containers/GraphClasses.h"

template<class T>
class EdgeInfoXML : public Edge<T> {
    WayInfoXML * info;

public:
    EdgeInfoXML(Vertex<T> * dest, double weight, WayInfoXML * info);
    WayInfoXML * getInfo() const;
    ~EdgeInfoXML();
};

template<class T>
EdgeInfoXML<T>::EdgeInfoXML(Vertex<T> *dest, double weight, WayInfoXML * info) : Edge<T>(dest, weight) {
    this->info = info;
    this->infoXML = true;
}

template<class T>
WayInfoXML * EdgeInfoXML<T>::getInfo() const {
    return info;
}

template<class T>
EdgeInfoXML<T>::~EdgeInfoXML<T>() {
    delete info;
}


#endif //SRC_GRAPHEDGEINFO_H
