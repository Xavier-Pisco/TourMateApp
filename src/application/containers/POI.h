#ifndef SRC_POI_H
#define SRC_POI_H

#include <cstring>
#include <map>

using namespace std;

/**
 * @brief Stores information about the Point of Interest
 */
class POI {
private:
    pair<double, double> coords; //!< pair with the coords (lat, long)
    string name; //!< name of the POI
    string description; //!< description of the POI
    string type; //!< type of the POI
    int averageSpentTime; //!< average time spent visiting the POI

public:
    /**
     * @brief Constructor for the Point of Interest object
     * @param lat - latitude
     * @param lon - longitude
     * @param n - name
     * @param d - description
     * @param avg - avg visit duration
     */
    POI(double lat, double lon, string n, string d, string type, int avg);

    /**
     * @brief GET method for the coords
     * @return pair with the coords
     */
    const pair<double, double> &getCoords() const;

    /**
     * @brief GET method for name
     * @return string name
     */
    const string &getName() const;

    /**
     * @brief GET method for description
     * @return string description
     */
    const string &getDescription() const;

    /**
     * @brief GET method for type
     * @return string type
     */
    const string &getType() const;

    /**
     * @brief GET method for avg visit time
     * @return avg visit time
     */
    int getAverageSpentTime() const;

};


#endif //SRC_POI_H
