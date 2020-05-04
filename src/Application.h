#ifndef SRC_APPLICATION_H
#define SRC_APPLICATION_H
#include "input/Converter.h"

class Application {
private:
    Graph<StreetIntersection, Road> * graph;
public:
    /**
     * @brief starts the application
     */
    void start();
};


#endif //SRC_APPLICATION_H
