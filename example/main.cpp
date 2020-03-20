#include <iostream>
#include <fstream>
#include <cstring>
#include <map>
#include "rapidxml/rapidxml_ext.h"
#include "graph/Graph.h"

/**
 *
 * This is just a test of how to use rapid xml to turn xml data into a graph
 *
 * */

using namespace std;

class XMLNode {
private:
    map<string, string> XMLNodeAttributes;

public:
    XMLNode(rapidxml::xml_node<> * node) {
        for (rapidxml::xml_attribute<> * attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            XMLNodeAttributes[attr->name()] = attr->value();
        }
    }

    const string &getID() const {
        return XMLNodeAttributes.at("id");
    }

    const map<string, string> &getXMLNodeAttributes() const {
        return XMLNodeAttributes;
    }
};


class StreetIntersection : public XMLNode {
private:
    unsigned count = 0;
public:
    explicit StreetIntersection(rapidxml::xml_node<> *node) : XMLNode(node) {}
    bool operator==(StreetIntersection &si) {
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
    map<string, string> xmlTags;

public:
    Road(rapidxml::xml_node<> * node) : XMLNode(node) {
        // looping through child nodes of way
        for (rapidxml::xml_node<> * n = node->first_node(); n; n = n->next_sibling()) {
            // if it refers to a node id
            if (strcmp(n->name(), "nd") == 0 && strcmp(n->first_attribute()->name(), "ref") == 0) {
                nodeIDs.emplace_back(n->first_attribute()->value());
            }
            // if it refers to a tag
            else if (strcmp(n->name(), "tag") == 0 && strcmp(n->first_attribute()->name(), "k") == 0 && strcmp(n->first_attribute()->next_attribute()->name(), "v") == 0) {
                xmlTags[n->first_attribute()->value()] = n->first_attribute()->next_attribute()->value();
            }
        }
    }

    const vector<string> &getNodeIDs() const {
        return nodeIDs;
    }

    const map<string, string> &getXMLTags() const {
        return xmlTags;
    }

    bool operator==(Road &r) {
        return this->getID() == r.getID();
    }
};


Graph<StreetIntersection, Road> parseXMLDocToGraph(rapidxml::xml_document<> &doc) {
    /*
     * ALGORITHM
     *
     * 1 - Sort out ways that are "highways" from others and increment their respective nodes count (by the node ID)
     *
     * 2 - Go through the filtered ways and:
     *          - A way usually becomes an edge EXCEPT when it passes through more nodes in between the beginning and ending node.
     *         In this case, the way is split into more edges.
     *          - Multiple edges with the same data (name, etc) mean that they are all part of the same road.
     *
     * 3 - to compute the weight of the edges (distance/avg time, etc) we need to go to each edge and evaluate the distance between its nodes
     *
     * IF THIS IS DONE CORRECTLY, IT SHOULD WORK PERFECTLY IN THEORY
     *
     * */

    /*
     * A NOTE ON XML_DOCUMENT
     *
     * It is composed of nodes. A node can have attributes and itself be composed of more nodes.
     *
     * [xml_document*]->first_node() or [xml_node*]->first_node() -> returns a pointer for the first child node of the document or node
     * [xml_node*]->next_sibling() -> returns a pointer to the next node inside the same parent (sibling)
     * [xml_node*]->first_attribute() -> returns the first attribute of the node
     * [xml_attribute*]->next_attribute() -> returns the next attribute of the same node
     *
     * All of these return 0 if the thing we're trying to access doesn't exist
     * There is also last_node and last_attribute
     *
     * */

    Graph<StreetIntersection, Road> res;

    map<string, StreetIntersection*> nodes;
    vector<Road*> roads;

    // looping through child nodes of osm node and adding them to nodes
    for (rapidxml::xml_node<> *node = doc.last_node()->first_node(); node; node = node->next_sibling()) {
        if (strcmp(node->name(), "node") == 0) {
            nodes[node->first_attribute()->value()] = new StreetIntersection(node);
        }
    }

    // looping though ways, incrementing StreetIntersection count and adding roads
    for (rapidxml::xml_node<> *node = doc.last_node()->first_node(); node; node = node->next_sibling()) {
        if (strcmp(node->name(), "way") == 0) {
            Road * candidate = new Road(node);
            /*cout << "## Candidate:" << endl;
            for (auto const &key : candidate->getXMLTags()) {
                cout<< key.first << " : " << key.second << endl;
            }*/
            bool done = false;
            for (auto const &key : candidate->getXMLTags()) {
                if (strcmp((char *) key.first.c_str(), "highway") == 0) {
                    roads.push_back(candidate);
                    for (const string& nodeID : roads.at(roads.size()-1)->getNodeIDs()) {
                        nodes[nodeID]->incrementCount();
                    }
                    done = true;
                    break;
                }
            }
            if (!done) delete candidate;
        }
    }

    // Erasing nodes with count == 0
    vector<string> k;
    for (auto & node : nodes) {
        if (node.second->getCount() == 0) {
            delete node.second;
            k.push_back(node.first);
        }
    }
    for (const string& key : k) {
        nodes.erase(key);
    }

    cout << "Roads: " << roads.size() << endl;
    cout << "Nodes: " << nodes.size() << endl;

    /* NEED TO DO STEP 2 AND 3 */

    return res;
}

int readFileData(const string &fileDir, string &data) {
    string line;
    ifstream mapFile(fileDir);

    if (mapFile.is_open())
    {
        while ( getline (mapFile,line) )
        {
            data += line + '\n';
        }
        mapFile.close();
    } else {
        return -1;
    }
    return 0;
}

rapidxml::xml_document<> * createXMLDoc(char * data) {
    auto * doc = new rapidxml::xml_document<>;
    (*doc).parse<0>(data);
    return doc;
}


int main() {

    string fileContent;
    if (readFileData("../input/map.osm", fileContent) != 0) {
        cout << "Error reading input file!" << endl;
        return -1;
    }
    rapidxml::xml_document<> * doc;
    doc = createXMLDoc((char*) fileContent.c_str());

    Graph<StreetIntersection, Road> myGraph = parseXMLDocToGraph(*doc);

    return 0;
}