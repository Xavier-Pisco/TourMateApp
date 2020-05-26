#ifndef SRC_PERFORMANCETESTS_H
#define SRC_PERFORMANCETESTS_H

#include <chrono>
#include "RouteMaker.h"

class PerformanceTests {
public:
    /**
     * @brief Performance tests menu
     */
    static void menu();

    /**
     * @brief GET method for the edge count of graph
     * @param g - the graph
     */
    template<class T>
    static int getEdgeCount(Graph<T> * g);

    /**
     * @brief runs dijkstra performance test in graph g
     * @param g - the graph
     * @return the time it took in microseconds
     */
    template<class T>
    static long unsigned runDijkstraTest(Graph<T> * g);

    /**
     * @brief runs Strongly Connected Components performance test in graph g
     * @param g - the graph
     * @return the time it took in microseconds
     */
    template<class T>
    static long unsigned runSCC(Graph<T> * g);

    /**
     * @brief runs the routemaker test, tests the route making algorithms
     * @param mapContainer - the mapContainer required for RouteMaker
     * @param t - the amount of maximum minutes the route must have
     * @return {{timeTaken, routeDist}, nPOIs}
     */
    static pair<pair<long unsigned, double>, int> runRouteMakerTest(SimpleMapContainer * mapContainer, int t);

    /**
     * @brief runs the routemaker test, tests the route making algorithm 3 times, finding the average and returning it
     * @param mapContainer - the mapContainer required for RouteMaker
     * @param t - the amount of maximum minutes the route must have
     * @return {{timeTaken, routeDist}, nPOIs} (average from 3 runs)
     */
    static pair<pair<long unsigned, double>, int> testRouteMakerAVG(SimpleMapContainer * mapContainer, int t);

    /**
     * @brief generates multiple route maker tests to be ran with testRouteMakerAVG and runRouteMakerTest and prints the results
     * @param mapContainer - the map container required for RouteMaker
     */
    static void testMultiples(SimpleMapContainer * mapContainer);

    /**
     * @brief prepares dijkstra test
     */
    static void testDijkstra();

    /**
     * @brief prepares SCC test
     */
    static void testSCC();

    /**
     * @brief prepares RouteMaker test
     * @param performanceTests - 0 if full, 1 if strong
     */
    static void testRouteMaker(int performanceTests);
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
