#ifndef SRC_GRAPHANALYZER_H
#define SRC_GRAPHANALYZER_H

#include "../graph/algorithms/scc.h"
#include "../ui/Menu.h"
#include "../ui/Drawer.h"

template<class T>
class GraphAnalyzer {
private:
    Graph<T> * graph;

public:
    explicit GraphAnalyzer(Graph<T> * graph);
    void menu();
    void stronglyConnectedComponents();
};

template<class T>
GraphAnalyzer<T>::GraphAnalyzer(Graph<T> * graph) {
    this->graph = graph;
}

template<class T>
void GraphAnalyzer<T>::menu() {
    Menu menu;
    menu.addOption("cancel");
    menu.addOption("Analyze strongly connected components");

    while(true) {
        Drawer::drawTitle("Graph analyzer");

        cout << endl;
        menu.draw();

        cout << endl;
        int opt = (int) menu.getResponse("Choose an option from the menu:");

        switch (opt) {
            case 0:
                return;
            case 1:
                stronglyConnectedComponents();
                break;
            default:
                break;
        }
    }
}


template<class T>
void GraphAnalyzer<T>::stronglyConnectedComponents() {
    vector<vector<Vertex<T>*>> v = graph->stronglyConnectedComponents();

    for (auto v1 : v) {
        for (Vertex<T> * vx : v1) {
            cout << vx->getInfo().getID() << "; ";
        }
        cout << endl;
    }
}


#endif //SRC_GRAPHANALYZER_H
