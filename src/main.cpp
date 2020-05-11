#include <iostream>
#include "Application.h"

using namespace std;

int main() {
    /*
    Application app(Application::RELEASE);
    app.start();
    */

    auto graph = Converter::getGraphFromTXTFile("../../cal-mapas-fornecidos/PortugalMaps/Porto/nodes_x_y_porto.txt", "../../cal-mapas-fornecidos/PortugalMaps/Porto/edges_porto.txt", "../../cal-mapas-fornecidos/TagExamples/Porto/t03_tags_porto.txt");
    graph->findVertex(1223751712)->coutPosition();
    cout << endl;
    graph->findVertex(1223751712)->coutType();

    return 0;
}
