#ifndef SRC_APPLICATION_H
#define SRC_APPLICATION_H

#include <string>

using namespace std;

class Application {
private:
public:
    /**
     * @brief the main menu
     * @return -1 to close app
     */
    int mainMenu();

    /**
     * @brief Converts lat to Y
     * @param lat - the lat
     * @param height - Y var
     * @param minCoords - min lat && lon
     * @param maxCoords - max lat && lon
     * @return the converted value
     */
    static int latToY(double lon, int height, pair<double, double> &minCoords, pair<double, double> &maxCoords);

    /**
     * @brief Converts lon to X
     * @param lon - the lon
     * @param width - X var
     * @param minCoords - min lat && lon
     * @param maxCoords - max lat && lon
     * @return the converted value
     */
    static int lonToX(double lon, int width, pair<double, double> &minCoords, pair<double, double> &maxCoords);
};


#endif //SRC_APPLICATION_H
