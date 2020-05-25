#ifndef SRC_GRAPHANALYZER_H
#define SRC_GRAPHANALYZER_H

#include "../input/UserInput.h"
#include "../graph/algorithms/findArticulationPoints.h"
#include "../graph/algorithms/scc.h"

template<class T>
class GraphAnalyzer {
private:
    Graph<T> * graph;

public:
    explicit GraphAnalyzer(Graph<T> * graph);
    void menu();
    void stronglyConnectedComponents();
    void articulationPoints();
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
    menu.addOption("Analyze articulation points");

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
            case 2:
                articulationPoints();
            default:
                break;
        }
    }
}


template<class T>
void GraphAnalyzer<T>::stronglyConnectedComponents() {
    vector<vector<Vertex<T>*>> v = graph->stronglyConnectedComponents();
    cout << endl;
    Drawer::drawTitle("Strongly Connected Components", 0, 80, true, "left");
    cout << "The strongly connected components are represented by the id's of their vertexes" << endl <<
        "Each line is a strongly connected component" << endl << endl;

    for (auto v1 : v) {
        for (Vertex<T> * vx : v1) {
            cout << vx->getInfo().getID() << "; ";
        }
        cout << endl;
    }

    cout<< endl << "Above, each line is a strongly connected component." << endl << endl;
    if (v.size() == 1) cout << "Only 1 strongly connected component, which means that the graph is strongly connected!" << endl;
    else cout << "The graph is not strongly connected" << endl;
    UserInput::waiter("");
}

template<class T>
void GraphAnalyzer<T>::articulationPoints() {
    vector<Vertex<T>*> v = graph->findArticulationPoints();
    cout << endl;
    Drawer::drawTitle("Articulation Points", 0, 80, true, "left");
    cout << "The articulation points are represented by their id" << endl << endl;
    int counter = 1;
    for (auto vx : v) {
        cout << vx->getInfo().getID() << "; ";
        if (counter % 15 == 0) cout << endl;
        counter++;
    }

    if (v.empty()) cout << endl << "No articulation points were found! the graph is biconnected" << endl;
    else cout << endl << "Above, each number is the id of an articulation point." << endl << "Therefore, the graph is not biconnected" << endl;
    UserInput::waiter("");
}


#endif //SRC_GRAPHANALYZER_H
