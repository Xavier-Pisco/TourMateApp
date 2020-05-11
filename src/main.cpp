#include <iostream>
#include "Application.h"

using namespace std;

int main() {
    /*
    Application app(Application::RELEASE);
    app.start();
    */

    auto graph = Converter::getGraphFromTXTFile("Porto");
    cout << graph->findVertex(495570435)->getInfo();

    return 0;
}
