#include <fstream>
#include <iostream>
#include "Converter.h"

Graph<StreetIntersection, Road> * Converter::getGraphFromOSMFile(const string& fileName) {
    string fileContent;
    if (readFileData(fileName, fileContent) != 0) {
        cout << "Error reading input file!" << endl;
        abort(); // this is temporary (or not)
    }
    rapidxml::xml_document<> * doc;
    doc = createXMLDoc((char*) fileContent.c_str());

    Graph<StreetIntersection, Road> * myGraph = parseXMLDocToGraph(*doc);

    return myGraph;
}


Graph<StreetIntersection, Road> * Converter::parseXMLDocToGraph(rapidxml::xml_document<> &doc) {
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

    auto * res = new Graph<StreetIntersection, Road>;

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
                if (strcmp((char *) key.first.c_str(), "highway") == 0) { // we are only interested in having ways that are roads
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

    int lastDone = -1;

    cout << endl << "Starting to add vertices..." << endl;
    int i = 1;
    for (auto & node : nodes) {
        if ((i*100)/nodes.size() % 10 == 0 && lastDone != (i*100)/nodes.size()) {
            cout << (i*100)/nodes.size() << "%" << " done\n";
            lastDone = (i*100)/nodes.size();
        }
        i++;
        res->addVertex(*node.second);
    }
    cout << "Finished adding vertices!" << endl;
    cout << endl << "Starting to add edges..." << endl;
    lastDone = -1;
    i = 1;
    for (Road * edge : roads) {
        if ((i*100)/roads.size() % 10 == 0 && lastDone != (i*100)/roads.size()) {
            cout << (i*100)/roads.size() << "%" << " done\n";
            lastDone = (i*100)/roads.size();
        }
        i++;

        const vector<string>& nodeIDs = edge->getNodeIDs();

        // Testing if oneway or not
        bool oneway = false;
        if (edge->getXMLTags().find("highway") != edge->getXMLTags().end()) {
            string highwayValue = edge->getXMLTags().at("highway");

            if (edge->getXMLTags().find("oneway") != edge->getXMLTags().end()) {
                if (edge->getXMLTags().at("oneway") == "yes" || edge->getXMLTags().at("oneway") == "1" ||
                    edge->getXMLTags().at("oneway") == "true") {
                    oneway = true;
                }
            } else if (highwayValue == "motorway" || highwayValue == "motorway_link" || highwayValue == "trunk_link" ||
                       highwayValue == "primary_link") {
                oneway = true;
            } else if (edge->getXMLTags().find("junction") != edge->getXMLTags().end()) {
                if (edge->getXMLTags().at("junction") == "roundabout") {
                    oneway = true;
                }
            }
        } else {
            oneway = true;
        }

        // simplest case
        if (nodeIDs.size() == 2) {
            if (nodes.find(nodeIDs.at(0)) != nodes.end() && nodes.find(nodeIDs.at(1)) != nodes.end()) {
                res->addEdge(*nodes.at(nodeIDs.at(0)), *nodes.at(nodeIDs.at(1)), *edge);
                if (!oneway) {
                    res->addEdge(*nodes.at(nodeIDs.at(1)), *nodes.at(nodeIDs.at(0)), *edge);
                }
            }
        } else {
            for (unsigned j = 0; j < nodeIDs.size()-1; j++) {
                if (nodes.find(nodeIDs.at(j)) != nodes.end() && nodes.find(nodeIDs.at(j+1)) != nodes.end()) {
                    res->addEdge(*nodes.at(nodeIDs.at(j)), *nodes.at(nodeIDs.at(j + 1)), *edge);
                    if (!oneway) {
                        res->addEdge(*nodes.at(nodeIDs.at(j + 1)), *nodes.at(nodeIDs.at(j)), *edge);
                    }
                }
            }
        }
    }
    cout << "Finished!" << endl << endl;

    return res;
}

int Converter::readFileData(const string &fileDir, string &data) {
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

rapidxml::xml_document<> * Converter::createXMLDoc(char * data) {
    auto * doc = new rapidxml::xml_document<>;
    (*doc).parse<0>(data);
    return doc;
}

