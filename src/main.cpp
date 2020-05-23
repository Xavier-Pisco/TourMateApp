#include "application/Application.h"

#include <csignal>

using namespace std;

int main() {
    signal(SIGINT, SIG_IGN);  // this is being used to ignore the SIGINT signal sent by GraphViewer

    Application app;
    app.mainMenu();

    getchar(); // this is so the program exits successfully (due to GraphViewer)
    return 0;
}
