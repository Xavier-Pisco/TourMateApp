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
    static int getInt(string getterPhrase, int lowerLimit = INT32_MIN, int upperLimit = INT32_MAX, bool cancelInput = true);

    /**
     * @brief gets confirmation from the user
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return true if user confirms, false if user cancels
     */
    static bool getConfirmation(string getterPhrase);

    /**
     * @brief waits for user input
     * @param getterPhrase
     */
    static void waiter(string getterPhrase);

    /**
     * @brief converts a string to lower case
     * @param s - the sting to convert
     * @return the converted string
     */
    static string strToLower(string s);

    /**
     * @brief GETs a vertex from client input
     * @param graph - the graph
     * @param mandatory - whether or not the client must choose a vertex
     * @return the vertex
     */
    template<class T>
    static Vertex<T> * getVertex(MapContainer<T> * graphContainer, bool locationName, bool mandatory = true);

    /**
     * @brief Gets a vertex from id input
     * @param mapContainer
     * @return the vertex
     */
    template<class T>
    static Vertex<T> * getVertexWithID(MapContainer<T> * mapContainer);

    /**
     * @brief GET's a vertex from lat lon input
     * @param mapContainer - the map container
     * @return the vertex
     */
    template<class T>
    static Vertex<T> * getVertexWithGPSCoords(MapContainer<T> * mapContainer);

    /**
     * @brief GET's a vertex from a street name
     * @param graph - the graph
     * @return the vertex
     */
    static Vertex<VertexInfoXML> * getVertexWithLocationName(MapContainer<VertexInfoXML> * graphContainer);

    static string getPreference();
};


template<class T>
Vertex<T> * UserInput::getVertex(MapContainer<T> * mapContainer, bool locationName, bool mandatory) {
    Menu menu;
    enum action {CANCEL, VID, LATLNG, NONE, NAME};
    menu.addOption("cancel", CANCEL);
    menu.addOption("add location with vertex id", VID);
    menu.addOption("add location with GPS coordinates", LATLNG);
    if (locationName) menu.addOption("add location with location name", NAME);
    if (!mandatory) menu.addOption("I do not need to specify", NONE);

    menu.drawMenuOptions("");
    cout << endl;
    unsigned opt = menu.getResponse("Choose an option from the menu:");


    switch(opt) {
        case CANCEL:
            throw CancelInput();
        case VID:
            return UserInput::getVertexWithID(mapContainer);
        case LATLNG:
            return UserInput::getVertexWithGPSCoords(mapContainer);
        case NAME:
            return (Vertex<T>*) UserInput::getVertexWithLocationName((OSMapContainer*)mapContainer); // this is only used in OSM maps
        case NONE:
        default:
            break;
    }
    return nullptr;
}

template<class T>
Vertex<T> * UserInput::getVertexWithID(MapContainer<T> * mapContainer) {
    cout << "This option finds the vertex with the ID you specify. \nInsert 'stop' to cancel at any time." << endl << endl;

    string getterPhrase = "Insert the id of the vertex:";
    while (true) {
        long id = UserInput::getInt(getterPhrase);

        Vertex<T> * v = mapContainer->getVertexWithID(id);
        if (v != nullptr) return v;
        else cout << "Insert a valid ID." << endl << endl;
    }


}

template<class T>
Vertex<T> * UserInput::getVertexWithGPSCoords(MapContainer<T> * mapContainer) {
    Coords coords;

    cout << "This option finds the vertex with the coordinates \nthat are the closest to the ones you specify" << endl << endl;

    coords.first = UserInput::getDouble("Latitude:");
    coords.second = UserInput::getDouble("Longitude:");

    return mapContainer->getVertexWithCoords(coords);
}

#endif //SRC_USERINPUT_H
