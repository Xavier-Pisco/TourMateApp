#include <iostream>
#include <fstream>
#include "rapidxml/rapidxml_ext.h"
#include "graph/Graph.h"
#include <cstring>

/**
 *
 * This is just a test of how to use rapid xml to turn xml data into a graph
 *
 * */

using namespace std;

class streetIntersection {
private:
    int id;
public:
    explicit streetIntersection(int id) {
        this->id = id;
    }

    int getID() {
        return id;
    }
};

int main() {
    Graph<streetIntersection> myGraph;

    ifstream mapFile("../input/map.osm");

    string fileContent;
    string line;

    if (mapFile.is_open())
    {
        while ( getline (mapFile,line) )
        {
            fileContent += line + '\n';
        }
        mapFile.close();
    }

    rapidxml::xml_document<> doc;
    doc.parse<0>((char*)fileContent.c_str());

    for (rapidxml::xml_node<> *node = doc.last_node()->first_node() /* This is the first node of the last node of th document = first node of osm = node */; node; node = node->next_sibling()) {
        if (strcmp(node->name(), "way") != 0) continue;
        cout << "-- Found a way!\n" << "Attributes: " << endl;
        for (rapidxml::xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
            cout << attr->name() << " : " << attr->value() << endl;
        }
        for (rapidxml::xml_node<> *n = node->first_node(); n; n = n->next_sibling()) {
            if (strcmp(n->name(), "tag") == 0) {
                for (rapidxml::xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute()) {
                    if (strcmp(attr->name(), "k") == 0 && strcmp(attr->value(), "highway") == 0) {
                        cout << "### It's a highway!" << endl;
                    }
                }
            }
        }
        cout << endl << endl;
    }

    return 0;
}