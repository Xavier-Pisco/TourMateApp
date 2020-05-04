#ifndef SRC_GRAPHCLASSES_H
#define SRC_GRAPHCLASSES_H

#include "../input/rapidxml/rapidxml.hpp"
#include "Graph.h"
#include <cstring>
#include <map>


/**
 * @brief Stores information about the Point of Interest
 */
class POI {
private:
    pair<double, double> coords; //!< pair with the coords (lat, long)
    string name; //!< name of the POI
    string description; //!< description of the POI
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
    POI(double lat, double lon, string n, string d, int avg);

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
     * @brief GET method for avg visit time
     * @return avg visit time
     */
    int getAverageSpentTime() const;

};

/**
 * @brief Stores information about XMLNodes (which aren't the same as the nodes in a graph)
 * An XMLNode is for example, one of the tags present in the XML file
 * (which may itself have more XMLNodes inside itself)
 */

class XMLNode {
private:
    map<string, string> XMLNodeAttributes; //!< map of (attributeName, attributeValue)
    map<string, string> xmlTags; //!< map of (tagName, tagValue)

public:
    /**
     * @brief Constructs the object
     * @param node - pointer to the xml_node
     */
    XMLNode(rapidxml::xml_node<> * node);

    /**
     * @brief GET method for ID
     * @return the id
     */
    const string &getID() const;

    /**
     * @brief GET method for the XML attributes that were present in the xml node
     * @return map with the attribute name and the value
     */
    const map<string, string> &getXMLNodeAttributes() const;

    /**
     * @brief GET method for the XML tags inside the node (subnodes). In this case they normally only have a name and a value
     * @return map with the name and the value of the tag
     */
    const map<string, string> &getXMLTags() const;
};

/**
 * @brief Stores information about a street intersection - correspondent to the node of the graph
 */
class StreetIntersection : public XMLNode {
private:
    unsigned count = 0; //!< count of roads that pass through this node
    POI * poi = nullptr; //!< pointer to the POI object
public:
    /**
     * @brief constructor for the object
     * @param node - pointer to the xml node
     */
    explicit StreetIntersection(rapidxml::xml_node<> *node);

    /**
     * @brief == operator overload against a const object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(const StreetIntersection &si);

    /**
     * @brief == operator overload against an object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(StreetIntersection &si);

    /**
     * @brief increments the count of edges that pass through this node
     */
    void incrementCount();

    /**
     * @brief GET method for the count
     * @return the vaalue of count
     */
    unsigned int getCount() const;
};

/**
 * @brief Stores information about a road - correspondent to the edge of the graph
 */
class Road : public XMLNode {
private:
    vector<string> nodeIDs; //!< vector with the ids of the nodes this road goes through
public:
    Road(rapidxml::xml_node<> * node);

    /**
     * @brief GET method for nodeIDs
     * @return the vector with the node IDs
     */
    const vector<string> &getNodeIDs() const;

    /**
     * @brief == operator overload against a const object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(const Road &r);

    /**
     * @brief == operator overload against an object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(Road &r);
};


#endif //SRC_GRAPHCLASSES_H
