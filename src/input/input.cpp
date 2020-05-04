#include <cstring>
#include <map>
#include "input.h"

/**
 *
 * This is just a test of how to use rapid xml to turn xml data into a graph
 *
 * */

using namespace std;

class POI {
private:
    pair<double, double> coords;
    string name;
    string description;
    int averageSpentTime;

public:
    POI(double lat, double lon, string n, string d, int avg) {
        coords.first = lat;
        coords.second = lon;
        name = n;
        description = d;
        averageSpentTime = avg;
    }

    const pair<double, double> &getCoords() const {
        return coords;
    }

    const string &getName() const {
        return name;
    }

    const string &getDescription() const {
        return description;
    }

    int getAverageSpentTime() const {
        return averageSpentTime;
    }

};

class XMLNode {
private:
    map<string, string> XMLNodeAttributes;
    map<string, string> xmlTags;

public:
    XMLNode(rapidxml::xml_node<> * node) {
        for (rapidxml::xml_attribute<> * attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            XMLNodeAttributes[attr->name()] = attr->value();
        }

        // looping through child nodes of xmlnode
        for (rapidxml::xml_node<> * n = node->first_node(); n; n = n->next_sibling()) {
            // if it refers to a tag
            if (strcmp(n->name(), "tag") == 0 && strcmp(n->first_attribute()->name(), "k") == 0 && strcmp(n->first_attribute()->next_attribute()->name(), "v") == 0) {
                xmlTags[n->first_attribute()->value()] = n->first_attribute()->next_attribute()->value();
            }
        }
    }

    const string &getID() const {
        return XMLNodeAttributes.at("id");
    }

    const map<string, string> &getXMLNodeAttributes() const {
        return XMLNodeAttributes;
    }

    const map<string, string> &getXMLTags() const {
        return xmlTags;
    }
};


class StreetIntersection : public XMLNode {
private:
    unsigned count = 0;
    POI * poi = nullptr;
public:
    explicit StreetIntersection(rapidxml::xml_node<> *node) : XMLNode(node) {}
    bool operator==(const StreetIntersection &si) {
        return this->getID() == si.getID();
    }

    bool operator==( StreetIntersection &si) {
        return this->getID() == si.getID();
    }

    void incrementCount() {
        count++;
    }

    unsigned int getCount() const {
        return count;
    }
};

class Road : public XMLNode {
private:
    vector<string> nodeIDs;
public:
    Road(rapidxml::xml_node<> * node) : XMLNode(node) {
        // looping through child nodes of way
        for (rapidxml::xml_node<> * n = node->first_node(); n; n = n->next_sibling()) {
            // if it refers to a node id
            if (strcmp(n->name(), "nd") == 0 && strcmp(n->first_attribute()->name(), "ref") == 0) {
                nodeIDs.emplace_back(n->first_attribute()->value());
            }
        }
    }

    const vector<string> &getNodeIDs() const {
        return nodeIDs;
    }

    bool operator==(const Road &r) {
        return this->getID() == r.getID();
    }

    bool operator==(Road &r) {
        return this->getID() == r.getID();
    }
};
