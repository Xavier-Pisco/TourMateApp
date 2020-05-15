#include "GraphClasses.h"


using namespace std;

/* POI */

POI::POI(double lat, double lon, string n, string d, int avg) {
    coords.first = lat;
    coords.second = lon;
    name = n;
    description = d;
    averageSpentTime = avg;
}

const pair<double, double> &POI::getCoords() const {
    return coords;
}

const string &POI::getName() const {
    return name;
}

const string &POI::getDescription() const {
    return description;
}

int POI::getAverageSpentTime() const {
    return averageSpentTime;
}

/* XMLNode */

XMLNode::XMLNode(rapidxml::xml_node<> * node) {
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

const string &XMLNode::getID() const {
    return XMLNodeAttributes.at("id");
}

const map<string, string> &XMLNode::getXMLNodeAttributes() const {
    return XMLNodeAttributes;
}

const map<string, string> &XMLNode::getXMLTags() const {
    return xmlTags;
}

/* RoadIntersection */

RoadIntersection::RoadIntersection(rapidxml::xml_node<> *node) : XMLNode(node) {}

bool RoadIntersection::operator==(const RoadIntersection &si) {
    return this->getID() == si.getID();
}

bool RoadIntersection::operator==(RoadIntersection &si) {
    return this->getID() == si.getID();
}

void RoadIntersection::incrementCount() {
    count++;
}

unsigned int RoadIntersection::getCount() const {
    return count;
}

/* Road */

Road::Road(rapidxml::xml_node<> * node) : XMLNode(node) {
    // looping through child nodes of way
    for (rapidxml::xml_node<> * n = node->first_node(); n; n = n->next_sibling()) {
        // if it refers to a node id
        if (strcmp(n->name(), "nd") == 0 && strcmp(n->first_attribute()->name(), "ref") == 0) {
            nodeIDs.emplace_back(n->first_attribute()->value());
        }
    }
}

const vector<string> &Road::getNodeIDs() const {
    return nodeIDs;
}

bool Road::operator==(const Road &r) {
    return this->getID() == r.getID();
}

bool Road::operator==(Road &r) {
    return this->getID() == r.getID();
}

double Road::getWeight() const {
    return this->weight;
}
