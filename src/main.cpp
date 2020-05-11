#include <iostream>
#include "Application.h"

using namespace std;

int main() {
    Application app(Application::DEBUG);
    /*
    Application app(Application::RELEASE);
    app.start();
    */

    auto graph = Converter::getGraphFromTXTFile("../../cal-mapas-fornecidos/PortugalMaps/Porto/nodes_x_y_porto.txt", "../../cal-mapas-fornecidos/PortugalMaps/Porto/edges_porto.txt", "../../cal-mapas-fornecidos/TagExamples/Porto/t03_tags_porto.txt");
    cout << graph->findVertex(495570435)->getInfo();

    return 0;
}
