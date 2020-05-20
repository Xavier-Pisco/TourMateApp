#ifndef SRC_USERINPUT_H
#define SRC_USERINPUT_H
#include <string>
#include <iostream>
#include <cmath>
#include <cfloat>
#include <map>
#include <sstream>
#include "../containers/MapContainer.h"
#include "../ui/Menu.h"
#include "../exceptions.h"

using namespace std;

class UserInput {
public:
    /**
     * @brief gets a line
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return line
     */
    static string getLine(string getterPhrase, bool cancelInput = true);

    /**
     * @brief gets a float number
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return number
     */
    static float getFloat(string getterPhrase, float lowerLimit = -FLT_MAX, float upperLimit = FLT_MAX, bool cancelInput = true);

    /**
     * @brief gets a double number
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return number
     */
    static double getDouble(string getterPhrase, double lowerLimit = -DBL_MAX, double upperLimit = DBL_MAX, bool cancelInput = true);

    /**
     * @brief gets an int number
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @param lowerLimit - the lower limit of the value
     * @param upperLimit - the upper limit of the value
     * @return number
     */
    static int getInt(string &getterPhrase, int lowerLimit = INT32_MIN, int upperLimit = INT32_MAX, bool cancelInput = true);

    /**
     * @brief gets confirmation from the user
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return true if user confirms, false if user cancels
     */
    static bool getConfirmation(string getterPhrase);

    /**
     * @brief converts a string to lower case
     * @param s - the sting to convert
     * @return the converted string
     */
    static string strToLower(string s);

    /**
     * @brief GET's a vertex from client input
     * @param graph - the graph
     * @param mandatory - whether or not the client must choose a vertex
     * @return the vertex
     */
    static Vertex<VertexInfoXML> * getVertex(MapContainer<VertexInfoXML> * graphContainer, bool mandatory = true);

    /**
     * @brief GET's a vertex from lat lon input
     * @param mapContainer - the map container
     * @return the vertex
     */
    static Vertex<VertexInfoXML> * getVertexWithGPSCoords(MapContainer<VertexInfoXML> * mapContainer);

    /**
     * @brief GET's a vertex from a street name
     * @param graph - the graph
     * @return the vertex
     */
    static Vertex<VertexInfoXML> * getVertexWithLocationName(MapContainer<VertexInfoXML> * graphContainer);

    static string getPreference();
};

#endif //SRC_USERINPUT_H
