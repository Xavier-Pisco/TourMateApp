#ifndef SRC_USERINPUT_H
#define SRC_USERINPUT_H
#include <string>
#include <iostream>
#include "../graph/Graph.h"
#include "../ui/Menu.h"
#include <cmath>
#include <cfloat>
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
    template<class T, class P>
    static Vertex<T, P> * getVertex(Graph<T, P> * graph, bool mandatory = true);

    /**
     * @brief GET's a vertex from lat lon input
     * @param graph - the graph
     * @return the vertex
     */
    template<class T, class P>
    static Vertex<T, P> * getVertexWithGPSCoords(Graph<T, P> * graph);

};


template<class T, class P>
Vertex<T, P> * UserInput::getVertex(Graph<T, P> * graph, bool mandatory) {
    Menu menu;
    menu.addOption("cancel");
    menu.addOption("add location with GPS coordinates");
    menu.addOption("add location with street name");
    menu.addOption("add location with POI name");
    if (!mandatory) menu.addOption("I do not need to specify");

    menu.drawMenuOptions("");
    cout << endl;
    int opt = menu.getResponse("Choose an option from the menu:");


    switch(opt) {
        case 0:
            throw CancelInput();
        case 1:
            return UserInput::getVertexWithGPSCoords(graph);
        case 2:
            //getVertexWithStreetName
            throw VertexNotFound();
            break;
        case 3:
            //getVertexWithPOIName
            throw VertexNotFound();
            break;
        case 4:
        default:
            break;
    }
    return nullptr;
}

template<class T, class P>
Vertex<T, P> * UserInput::getVertexWithGPSCoords(Graph<T, P> * graph) {
    pair<double, double> coords;

    cout << "This option finds the vertex with the coordinates \nthat are the closest to the ones you specify" << endl << endl;

    coords.first = UserInput::getDouble("Latitude:");
    coords.second = UserInput::getDouble("Longitude:");

    //cout << "lat: " << coords.first << " lon: " << coords.second << endl;

    pair<Vertex<T, P> *, double> vertexWithDist;
    vertexWithDist.second = DBL_MAX;

    for (Vertex<T, P> * v : graph->getVertexSet()) {
        double dist = sqrt( pow(v->getInfo().getLat() - coords.first, 2) + pow(v->getInfo().getLon() - coords.second, 2) );
        if (dist < vertexWithDist.second) {
            //cout << dist << endl;
            vertexWithDist.first = v;
            vertexWithDist.second = dist;
        }
    }

    return vertexWithDist.first;
}

#endif //SRC_USERINPUT_H
