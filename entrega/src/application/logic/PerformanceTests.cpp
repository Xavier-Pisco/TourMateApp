#include "PerformanceTests.h"


void PerformanceTests::menu() {
    while (true) {
        Menu menu;
        menu.addOption("cancel");
        menu.addOption("Test Dijkstra");
        menu.addOption("Test Strongly Connected Components");
        menu.addOption("Test Route Maker");


        cout << endl;
        menu.draw("");

        unsigned opt = menu.getResponse("Choose an option from the menu:");
        switch(opt) {
            case 0:
                return;
            case 1:
                testDijkstra();
                break;
            case 2:
                testSCC();
                break;
            case 3:
                testRouteMaker();
                break;
            default:
                break;
        }
    }

}

pair<pair<long unsigned, double>, int> PerformanceTests::runRouteMakerTest(SimpleMapContainer * mapContainer, int t) {
    SimpleRouteMaker * routeMaker = new SimpleRouteMaker();
    routeMaker->setMapContainer(mapContainer);

    User<VertexInfoTXT> user;
    user.setRouteMode(User<VertexInfoTXT>::CAR);
    user.setAvailability(t);
    for (auto s : mapContainer->getAvailableCategories()) {
        user.addPreference(s);
    }
    vector<Vertex<VertexInfoTXT>*> vxSet = mapContainer->getGraph()->getVertexSet();
    srand(time(nullptr));
    user.setOrigin(vxSet.at(rand()%vxSet.size()));
    user.setDestination(vxSet.at(rand()%vxSet.size()));

    routeMaker->setUser(user);

    auto start = chrono::high_resolution_clock::now();

    int poiCount = routeMaker->fillExtraTimeRoute();

    auto finish = chrono::high_resolution_clock::now();
    pair<pair<long unsigned, double>, int> res;
    res.first.first = chrono::duration_cast<chrono::microseconds>(finish - start).count();
    res.first.second = routeMaker->getRouteDist();
    res.second = poiCount;
    return res;
}

pair<pair<long unsigned, double>, int> PerformanceTests::testRouteMakerAVG(SimpleMapContainer * mapContainer, int t) {
    pair<pair<long unsigned, double>, int> elapsed, tmp;
    elapsed = {{0, 0}, 0};
    for (int i = 0; i < 3; i++) {
        tmp = runRouteMakerTest(mapContainer, t);
        elapsed.first.first += tmp.first.first;
        elapsed.first.second += tmp.first.second;
        elapsed.second += tmp.second;
    }
    elapsed.first.first = elapsed.first.first/3;
    elapsed.first.second = elapsed.first.second/3;
    elapsed.second = elapsed.second/3;
    return elapsed;
}

void PerformanceTests::testMultiples(SimpleMapContainer * mapContainer) {
    pair<pair<long unsigned, double>, int> elapsed;
    for (int i = 1; i <= 5; i++) {
        elapsed = testRouteMakerAVG(mapContainer, i*100);
        cout << "Test " << i << ": Availaible time: " << i*100 << " mins, number of POIs visited: " << elapsed.second << ", route distance: "
             << elapsed.first.second << "km. " << "Took " << elapsed.first.first << " microseconds." << endl;
    }
}

void PerformanceTests::testRouteMaker() {
    // these tests run on the maps with strong connectivity
    cout << "Running RouteMaker tests" << endl;

    string map = "../maps/simple/penafiel";
    auto * mapContainer = new SimpleMapContainer(map, true);

    int vxSetSize = mapContainer->getGraph()->getVertexSet().size();
    int edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Penafiel test: " << vxSetSize << " vertexes, " << edgeCount << " edges." << endl;
    testMultiples(mapContainer);


    map = "../maps/simple/espinho";
    mapContainer = new SimpleMapContainer(map, true);

    vxSetSize = mapContainer->getGraph()->getVertexSet().size();
    edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Espinho test: " << vxSetSize << " vertexes, " << edgeCount << " edges." << endl;
    testMultiples(mapContainer);


    map = "../maps/simple/porto";
    mapContainer = new SimpleMapContainer(map, true);

    vxSetSize = mapContainer->getGraph()->getVertexSet().size();
    edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Porto test: " << vxSetSize << " vertexes, " << edgeCount << " edges." << endl;
    testMultiples(mapContainer);

    delete mapContainer;
}


void PerformanceTests::testDijkstra() {
    // these tests run on the maps with strong connectivity
    cout << "Running Dijkstra tests" << endl;

    string map = "../maps/simple/penafiel";
    auto * mapContainer = new SimpleMapContainer(map, true);

    auto elapsed = runDijkstraTest(mapContainer->getGraph());
    int edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Finished Penafiel: " << mapContainer->getGraph()->getVertexSet().size() << " vertexes, " <<
        edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    delete mapContainer;

    map = "../maps/simple/espinho";
    mapContainer = new SimpleMapContainer(map, true);

    elapsed = runDijkstraTest(mapContainer->getGraph());
    edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Finished Espinho: " << mapContainer->getGraph()->getVertexSet().size() << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    delete mapContainer;

    map = "../maps/simple/porto";
    mapContainer = new SimpleMapContainer(map, true);

    elapsed = runDijkstraTest(mapContainer->getGraph());
    edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Finished Porto: " << mapContainer->getGraph()->getVertexSet().size() << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    delete mapContainer;
}


void PerformanceTests::testSCC() {
    // these tests run on the maps with strong connectivity
    cout << "Running Strongly Connected Components tests" << endl;

    string map = "../maps/simple/penafiel";
    auto * mapContainer = new SimpleMapContainer(map, true);

    auto elapsed = runSCC(mapContainer->getGraph());
    int edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Finished Penafiel: " << mapContainer->getGraph()->getVertexSet().size() << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    delete mapContainer;

    map = "../maps/simple/espinho";
    mapContainer = new SimpleMapContainer(map, true);

    elapsed = runSCC(mapContainer->getGraph());
    edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Finished Espinho: " << mapContainer->getGraph()->getVertexSet().size() << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    delete mapContainer;

    map = "../maps/simple/porto";
    mapContainer = new SimpleMapContainer(map, true);

    elapsed = runSCC(mapContainer->getGraph());
    edgeCount = getEdgeCount(mapContainer->getGraph());

    cout << "Finished Porto: " << mapContainer->getGraph()->getVertexSet().size() << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    delete mapContainer;
}
