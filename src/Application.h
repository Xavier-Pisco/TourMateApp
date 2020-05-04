#ifndef SRC_APPLICATION_H
#define SRC_APPLICATION_H
#include "input/Converter.h"

class Application {
private:
    Graph<StreetIntersection, Road> * graph;
public:
    void init();
};


#endif //SRC_APPLICATION_H
