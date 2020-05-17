#include "GraphClasses.h"


using namespace std;


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

long XMLNode::getID() const {
    if (XMLNodeAttributes.find("id") == XMLNodeAttributes.end()) {
        cout << "Couldn't find id" << endl;
    }
    return stol(XMLNodeAttributes.at("id"));
}

const map<string, string> &XMLNode::getXMLNodeAttributes() const {
    return XMLNodeAttributes;
}

const map<string, string> &XMLNode::getXMLTags() const {
    return xmlTags;
}

/* VertexInfoXML */

VertexInfoXML::VertexInfoXML(rapidxml::xml_node<> *node) : XMLNode(node) {}

bool VertexInfoXML::operator==(const VertexInfoXML &si) {
    return this->getID() == si.getID();
}

bool VertexInfoXML::operator==(VertexInfoXML &si) {
    return this->getID() == si.getID();
}

void VertexInfoXML::incrementCount() {
    count++;
}

unsigned int VertexInfoXML::getCount() const {
    return count;
}

double VertexInfoXML::getLat() const {
    return stod(XMLNodeAttributes.at("lat"));
}

double VertexInfoXML::getLon() const {
    return stod(XMLNodeAttributes.at("lon"));
}

long VertexInfoXML::getID() const {
    return XMLNode::getID();
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
