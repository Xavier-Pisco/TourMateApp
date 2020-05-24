#ifndef SRC_PERFORMANCETESTS_H
#define SRC_PERFORMANCETESTS_H

#include <chrono>
#include "RouteMaker.h"

class PerformanceTests {
public:
    static void menu();

    template<class T>
    static int getEdgeCount(Graph<T> * g);

    template<class T>
    static long unsigned runDijkstraTest(Graph<T> * g);

    template<class T>
    static long unsigned runSCC(Graph<T> * g);

    static long unsigned runRouteMakerTest1(SimpleMapContainer * mapContainer);

    static void testDijkstra();
    static void testSCC();
    static void testRouteMaker();
};

template<class T>
int PerformanceTests::getEdgeCount(Graph<T> * g) {
    int edgeCount = 0;
    for (auto v : g->getVertexSet()) {
        for (auto e : v->getAdj()) edgeCount++;
    }
    return edgeCount;
}

template<class T>
long unsigned PerformanceTests::runDijkstraTest(Graph<T> * g) {
    auto start = chrono::high_resolution_clock::now();

    g->dijkstra(g->getVertexSet().at(0));

    auto finish = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(finish - start).count();
}

template<class T>
long unsigned PerformanceTests::runSCC(Graph<T> * g) {
    auto start = chrono::high_resolution_clock::now();

    g->stronglyConnectedComponents();

    auto finish = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(finish - start).count();
}


#endif //SRC_PERFORMANCETESTS_H
