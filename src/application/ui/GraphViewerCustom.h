#ifndef SRC_GRAPHVIEWERCUSTOM_H
#define SRC_GRAPHVIEWERCUSTOM_H

#include "../../lib/GraphViewer/graphviewer.h"
#include "../Application.h"
#include "../containers/GraphClasses.h"

class GraphViewerCustom {
private:
    Graph<VertexInfoXML> * graph;
    GraphViewer * graphViewer = nullptr;

public:
    explicit GraphViewerCustom(Graph<VertexInfoXML> * graph);
    void viewGraph();
    void closeView();
    ~GraphViewerCustom();
};


#endif //SRC_GRAPHVIEWERCUSTOM_H
