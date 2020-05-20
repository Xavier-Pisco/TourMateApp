#ifndef SRC_GRAPHCLASSES_H
#define SRC_GRAPHCLASSES_H

#include "../../lib/RapidXML/rapidxml.hpp"
#include "../graph/Graph.h"
#include "POI.h"
#include "Position.h"
#include <iostream>
#include <vector>


class VertexInfo {
public:
    virtual string getCategory() const {
        return "";
    }
    virtual double getLat() const = 0;
    virtual double getLon() const = 0;
    virtual long getID() const = 0;

    friend bool operator==(const VertexInfo& r1, const VertexInfo& r2){
        return r1.getID() == r2.getID();
    }
};


/**
 * @brief Stores information about XMLNodes (which aren't the same as the nodes in a graph)
 * An XMLNode is for example, one of the tags present in the XML file
 * (which may itself have more XMLNodes inside itself)
 */

class XMLNode {
protected:
    map<string, string> XMLNodeAttributes; //!< map of (attributeName, attributeValue)
    map<string, string> xmlTags; //!< map of (tagName, tagValue)

public:
    /**
     * @brief Constructs the object
     * @param node - pointer to the xml_node
     */
    explicit XMLNode(rapidxml::xml_node<> * node);

    /**
     * @brief GET method for ID
     * @return the id
     */
    long getID() const;

    /**
     * @brief GET method for the XML attributes that were present in the xml node
     * @return map with the attribute name and the value
     */
    const map<string, string> &getXMLAttributes() const;

    /**
     * @brief GET method for the XML tags inside the node (subnodes). In this case they normally only have a name and a value
     * @return map with the name and the value of the tag
     */
    const map<string, string> &getXMLTags() const;

    /**
     * @brief GET method for one XML attribute that may be present in the xml node
     * @return the value, empty string if not found
     */
    string getXMLAttributeValue(string attr) const;

    /**
     * @brief GET method for one XML tag that may be present in the tags of the xml node
     * @return the value, empty string if not found
     */
    string getXMLTagValue(string attr) const;
};

/**
 * @brief Stores information about a street intersection coming from XML - correspondent to the node of the graph
 */
class VertexInfoXML : public XMLNode, public VertexInfo {
private:
    unsigned count = 0; //!< count of roads that pass through this node
    POI * poi = nullptr; //!< pointer to the POI object
public:
    /**
     * @brief constructor for the object
     * @param node - pointer to the xml node
     */
    explicit VertexInfoXML(rapidxml::xml_node<> *node);

    /**
     * @brief == operator overload against a const object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(const VertexInfoXML &si);

    /**
     * @brief == operator overload against an object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(VertexInfoXML &si);

    /**
     * @brief increments the count of edges that pass through this node
     */
    void incrementCount();

    /**
     * @brief GET method for the count
     * @return the vaalue of count
     */
    unsigned int getCount() const;

    double getLat() const override;

    double getLon() const override;

    long getID() const override;
};

/**
 * @brief Stores information about a street intersection coming from TXT - correspondent to the node of the graph
 */
class VertexInfoTXT : public VertexInfo {
private:
    Position p;
    const long id;
    string category;
public:
    VertexInfoTXT(const long id, Position p) : id(id), p(p) {}

    friend ostream& operator <<(ostream &os, const VertexInfoTXT &r){
        os << r.id << " - (" << r.p.getLat() << ", " << r.p.getLon() << ") - " << r.category << endl;
        return os;
    }

    void setCategory(string &c){
        this->category = c;
    }

    string getCategory() const override {
        return category;
    }

    double getLat() const override {
        return p.getLat();
    }

    double getLon() const override {
        return p.getLon();
    }

    long getID() const override {
        return id;
    }
};



/**
 * @brief Stores information about a road - correspondent to the edge of the graph
 */
class WayInfoXML : public XMLNode {
private:
    vector<string> nodeIDs; //!< vector with the ids of the nodes this road goes through
    vector<Vertex<VertexInfoXML> *> vertexes;
    pair<double, double> apprCoords;
public:
    WayInfoXML(rapidxml::xml_node<> * node, map<string, Vertex<VertexInfoXML> *> &nodes);

    /**
     * @brief GET method for nodeIDs
     * @return the vector with the node IDs
     */
    const vector<string> &getNodeIDs() const;

    /**
     * @brief GET method for vertexes
     * @return the vertexes
     */
    const vector<Vertex<VertexInfoXML>*> &getVertexes() const;

    /**
     * @brief GET method for the approximate coords
     * @return the approximate coords
     */
    const pair<double, double> &getApprCoords() const;

    /**
     * @brief == operator overload against a const object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(const WayInfoXML &r);

    /**
     * @brief == operator overload against an object of this class
     * @param si - the object that this will be compared to
     * @return if its equal or not
     */
    bool operator==(WayInfoXML &r);
};


#endif //SRC_GRAPHCLASSES_H
