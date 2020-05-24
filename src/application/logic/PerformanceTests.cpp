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

long unsigned PerformanceTests::runRouteMakerTest1(SimpleMapContainer * mapContainer) {

    SimpleRouteMaker * routeMaker = new SimpleRouteMaker();
    routeMaker->setMapContainer(mapContainer);

    User<VertexInfoTXT> user;
    user.setRouteMode(User<VertexInfoTXT>::CAR);
    user.setAvailability(200);
    for (auto s : mapContainer->getAvailableCategories()) {
        user.addPreference(s);
    }
    vector<Vertex<VertexInfoTXT>*> vxSet = mapContainer->getGraph()->getVertexSet();
    srand(time(nullptr));
    user.setOrigin(vxSet.at(rand()%vxSet.size()));
    user.setDestination(vxSet.at(rand()%vxSet.size()));

    routeMaker->setUser(user);

    auto start = chrono::high_resolution_clock::now();

    routeMaker->fillExtraTimeRoute();

    auto finish = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::microseconds>(finish - start).count();
}

void PerformanceTests::testRouteMaker() {
    // these tests run on the maps with strong connectivity
    cout << "Running RouteMaker tests" << endl;

    string map = "../maps/simple/penafiel";
    auto * mapContainer = new SimpleMapContainer(map, true);

    int vxSetSize = mapContainer->getGraph()->getVertexSet().size();
    int edgeCount = getEdgeCount(mapContainer->getGraph());
    auto elapsed = runRouteMakerTest1(mapContainer);

    cout << "Finished Penafiel: " << vxSetSize << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    //delete mapContainer;

    map = "../maps/simple/espinho";
    mapContainer = new SimpleMapContainer(map, true);

    vxSetSize = mapContainer->getGraph()->getVertexSet().size();
    edgeCount = getEdgeCount(mapContainer->getGraph());
    elapsed = runRouteMakerTest1(mapContainer);

    cout << "Finished Espinho: " << vxSetSize << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    //delete mapContainer;

    map = "../maps/simple/porto";
    mapContainer = new SimpleMapContainer(map, true);

    vxSetSize = mapContainer->getGraph()->getVertexSet().size();
    edgeCount = getEdgeCount(mapContainer->getGraph());
    elapsed = runRouteMakerTest1(mapContainer);

    cout << "Finished Porto: " << vxSetSize << " vertexes, " <<
         edgeCount << " edges. Took " << elapsed << " microseconds." << endl;

    //delete mapContainer;
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
