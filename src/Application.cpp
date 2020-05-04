#include <iostream>
#include "Application.h"

void Application::start() {
    graph = Converter::getGraphFromOSMFile("../maps/centro_aliados.osm");

    int count = 1;
    for (const StreetIntersection& si : graph->dfs()) {
        if (si.getXMLTags().find("name") != si.getXMLTags().end()) {
            cout << "### Place" << endl;
            for (const auto& i : si.getXMLTags()) {
                cout << i.first << " : " << i.second << endl;
            }
            cout << endl << endl;
        }
        count++;
    }
}
