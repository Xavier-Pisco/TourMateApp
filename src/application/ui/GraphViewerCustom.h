#ifndef SRC_GRAPHVIEWERCUSTOM_H
#define SRC_GRAPHVIEWERCUSTOM_H

#include "../containers/GraphClasses.h"
#include "../../lib/GraphViewer/graphviewer.h"
#include "../Application.h"

class GraphViewerCustom {
private:
    Graph<VertexInfoXML, WayInfoXML> * graph;
    GraphViewer * graphViewer = nullptr;

public:
    explicit GraphViewerCustom(Graph<VertexInfoXML, WayInfoXML> * graph);
    void viewGraph();
    void closeView();
    ~GraphViewerCustom();
};


#endif //SRC_GRAPHVIEWERCUSTOM_H
