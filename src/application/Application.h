#ifndef SRC_APPLICATION_H
#define SRC_APPLICATION_H

#include <string>

using namespace std;

class Application {
public:

    /**
     * @brief starts the application
     */
    void start();

    /**
     * @brief the main menu
     * @return -1 to close app
     */
    int mainMenu();

    /**
     * @brief exits the application
     */
    void exit();

    static int latToY(const string &lon, int height, pair<double, double> &minCoords, pair<double, double> &maxCoords);
    static int lonToX(const string &lon, int height, pair<double, double> &minCoords, pair<double, double> &maxCoords);
};


#endif //SRC_APPLICATION_H
