#ifndef SRC_APPLICATION_H
#define SRC_APPLICATION_H
#include "input/Converter.h"
#include "lib/GraphViewer/graphviewer.h"

class Application {
public:
    enum MODE {DEBUG, RELEASE}; //!< mode in which to run the application
private:
    MODE mode;
    Graph<StreetIntersection, Road> * graph;
    GraphViewer * graphViewer;

public:
    /**
     * @brief Constructor
     * @param mode - the mode in which to run
     */
    explicit Application(MODE mode);

    /**
     * @brief starts the application
     */
    void start();

    /**
     * @brief uses GraphViewer to display graph
     */
    void viewGraph();
};


#endif //SRC_APPLICATION_H
