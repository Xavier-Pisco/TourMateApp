#ifndef SRC_GRAPHVIEWERCUSTOM_H
#define SRC_GRAPHVIEWERCUSTOM_H

#include "../../lib/GraphViewer/graphviewer.h"
#include "../Application.h"
#include "../containers/GraphClasses.h"

template<class T>
class GraphViewerCustom {
private:
    Graph<T> * graph;
    GraphViewer * graphViewer = nullptr;

public:
    explicit GraphViewerCustom(Graph<T> * graph);
    void viewGraph();
    void closeView();
    ~GraphViewerCustom();
};


#endif //SRC_GRAPHVIEWERCUSTOM_H
