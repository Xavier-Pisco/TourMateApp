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

const map<string, string> &XMLNode::getXMLAttributes() const {
    return XMLNodeAttributes;
}

const map<string, string> &XMLNode::getXMLTags() const {
    return xmlTags;
}

string XMLNode::getXMLAttributeValue(string attr) const {
    map<string, string>::const_iterator it;
    return (it = XMLNodeAttributes.find(attr)) != XMLNodeAttributes.end() ? it->second : "";
}

string XMLNode::getXMLTagValue(string attr) const {
    map<string, string>::const_iterator it;
    return (it = xmlTags.find(attr)) != xmlTags.end() ? it->second : "";
}

/* VertexInfoXML */

VertexInfoXML::VertexInfoXML(rapidxml::xml_node<> *node) : XMLNode(node) {}

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

/* WayInfoXML */

WayInfoXML::WayInfoXML(rapidxml::xml_node<> * node, map<string, Vertex<VertexInfoXML> *> &nodes) : XMLNode(node) {
    // looping through child nodes of way
    for (rapidxml::xml_node<> * n = node->first_node(); n; n = n->next_sibling()) {
        // if it refers to a node id
        if (strcmp(n->name(), "nd") == 0 && strcmp(n->first_attribute()->name(), "ref") == 0) {
            nodeIDs.emplace_back(n->first_attribute()->value());
            vertexes.emplace_back(nodes.find(n->first_attribute()->value())->second);
        }
    }

    apprCoords = {0, 0};

    for (Vertex<VertexInfoXML> * v : vertexes) {
        apprCoords.first += v->getInfo().getLat();
        apprCoords.second += v->getInfo().getLon();
    }

    apprCoords.first = apprCoords.first / (double) vertexes.size();
    apprCoords.second = apprCoords.second / (double) vertexes.size();
}

const vector<string> &WayInfoXML::getNodeIDs() const {
    return nodeIDs;
}

const vector<Vertex<VertexInfoXML>*> &WayInfoXML::getVertexes() const {
    return vertexes;
}

const pair<double, double> &WayInfoXML::getApprCoords() const {
    return apprCoords;
}

bool WayInfoXML::operator==(const WayInfoXML &r) {
    return this->getID() == r.getID();
}

bool WayInfoXML::operator==(WayInfoXML &r) {
    return this->getID() == r.getID();
}
