#include "Converter.h"

Graph<VertexInfoXML, Road> * Converter::getGraphFromOSMFile(const string& fileName) {
    string fileContent;
    if (readFileData(fileName, fileContent) != 0) {
        cout << "Error reading input file!" << endl;
        abort(); // this is temporary (or not)
    }
    rapidxml::xml_document<> * doc;
    doc = createXMLDoc((char*) fileContent.c_str());

    Graph<VertexInfoXML, Road> * myGraph = parseXMLDocToGraph(*doc);

    return myGraph;
}


Graph<VertexInfoXML, Road> * Converter::parseXMLDocToGraph(rapidxml::xml_document<> &doc) {
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

    auto * res = new Graph<VertexInfoXML, Road>;

    map<string, Vertex<VertexInfoXML, Road> *> nodes;
    vector<Road*> roads;

    // looping through child nodes of osm node and adding them to nodes
    for (rapidxml::xml_node<> *node = doc.last_node()->first_node(); node; node = node->next_sibling()) {
        if (strcmp(node->name(), "node") == 0) {
            nodes[node->first_attribute()->value()] = new Vertex<VertexInfoXML, Road>(VertexInfoXML(node));
        }
    }

    // looping though ways, incrementing VertexInfoXML count and adding roads
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
                        if (nodes.find(nodeID) != nodes.end())
                            nodes[nodeID]->getInfo().incrementCount();
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
        if (node.second->getInfo().getCount() == 0) {
            delete node.second;
            k.push_back(node.first);
        }
    }
    for (const string& key : k) {
        nodes.erase(key);
    }

    for (auto & node : nodes) {
        res->addVertex(node.second);
    }

    for (Road * edge : roads) {
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
        double length;
        map<string, Vertex<VertexInfoXML, Road>*>::iterator n1, n2;
        if (nodeIDs.size() == 2) {
            if ((n1 = nodes.find(nodeIDs.at(0))) != nodes.end() && (n2 = nodes.find(nodeIDs.at(1))) != nodes.end()) {
                length = getKmDistfromLatLong(n1->second->getInfo().getLat(), n1->second->getInfo().getLon(), n2->second->getInfo().getLat(), n2->second->getInfo().getLon());
                res->addEdge(n1->second, n2->second, *edge, length);
                if (!oneway) {
                    res->addEdge(n2->second, n1->second, *edge, length);
                }
            }
        } else {
            for (unsigned j = 0; j < nodeIDs.size()-1; j++) {
                if ((n1 = nodes.find(nodeIDs.at(j))) != nodes.end() && (n2 = nodes.find(nodeIDs.at(j+1))) != nodes.end()) {
                    length = getKmDistfromLatLong(n1->second->getInfo().getLat(), n1->second->getInfo().getLon(), n2->second->getInfo().getLat(), n2->second->getInfo().getLon());
                    res->addEdge(n1->second, n2->second, *edge, length);
                    if (!oneway) {
                        res->addEdge(n2->second, n1->second, *edge, length);
                    }
                }
            }
        }
    }

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

double Converter::degreesToRadians(double degrees) {
    return degrees * M_PI / 180.;
}

double Converter::getKmDistfromLatLong(double lat1, double lon1, double lat2, double lon2) {
    // haversine formula http://www.movable-type.co.uk/scripts/latlong.html

    int earthRadius = 6371; // in km

    double dLat = degreesToRadians(lat2 - lat1);
    double dLon = degreesToRadians(lon2 - lon1);

    lat1 = degreesToRadians(lat1);
    lat2 = degreesToRadians(lat2);

    double a = sin(dLat/2) * sin(dLat/2) + sin(dLon/2) * sin(dLon/2) * cos(lat1) * cos(lat2);

    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return c * earthRadius;
}


// TODO This needs to be done with a
//  graph that only has the T template argument. Otherwise, it won't work well, because the P template
//  argument is for the road information (name, etc) and that information isn't available here.
//  The edge weight is saved not in the P template argument, because multiple edges may share the P template argument,
//  but in a separate field of the Edge class, named 'weight'.
/*Graph<VertexInfoTXT, int> *Converter::getGraphFromTXTFile(const string & city) {
    auto graph = new Graph<VertexInfoTXT, int>;

    string lowerCaseCity = city;
    transform(lowerCaseCity.begin(), lowerCaseCity.end(), lowerCaseCity.begin(), ::tolower);

    string nodesFileName = "../../cal-mapas-fornecidos/PortugalMaps/" + city + "/nodes_x_y_" + lowerCaseCity + ".txt";
    string edgesFileName = "../../cal-mapas-fornecidos/PortugalMaps/" + city + "/edges_" + lowerCaseCity + ".txt";
    string poiFolderName = "../../cal-mapas-fornecidos/TagExamples/" + city + "/";

    readNodeFileTxt(nodesFileName, graph);

    readEdgesFileTxt(edgesFileName, graph);

    readTagsFromFolder(poiFolderName, graph);

    return graph;
}

vector<double> Converter::parseNodeLineToInts(string &line) {
    vector<double> v;
    line.erase(0, 1);
    v.push_back(stoi(line.substr(0, line.find(','))));
    line.erase(0, line.find(',') + 1);
    v.push_back(stod(line.substr(0, line.find(','))));
    line.erase(0, line.find(',') + 1);
    v.push_back(stod(line.substr(0, line.find(')'))));
    return v;
}

vector<double> Converter::parseEdgeLineToInts(string &line) {
    vector<double> v;
    line.erase(0, 1);
    v.push_back(stoi(line.substr(0, line.find(','))));
    line.erase(0, line.find(',') + 1);
    v.push_back(stoi(line.substr(0, line.find(','))));
    return v;
}

void Converter::readNodeFileTxt(const string &fileName, Graph<VertexInfoTXT, int> *graph) {
    string line;
    ifstream nodesFile(fileName);

    if (nodesFile.is_open()){
        getline(nodesFile, line);
        unsigned nodes_number = stoi(line);
        while (getline(nodesFile, line)) {
            vector<double> v = parseNodeLineToInts(line);
            unsigned id = v[0];
            Position p = Position(v[1], v[2]);
            graph->addVertex(VertexInfoTXT(id, p));
        }
    } else {
        abort();
    }

    nodesFile.close();
}

void Converter::readEdgesFileTxt(const string &fileName, Graph<VertexInfoTXT, int> *graph) {
    string line;
    ifstream edgesFile(fileName);

    if (edgesFile.is_open()){
        getline(edgesFile, line);
        unsigned edges_number = stoi(line);
        while(getline(edgesFile, line)){
            vector<double> v = parseEdgeLineToInts(line);
            //graph->addEdge(v[0], v[1], 1);
        }
    } else {
        abort();
    }

    edgesFile.close();
}

void Converter::readTagsFromFolder(const string &folderName, Graph<VertexInfoTXT, int> *graph) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(folderName.c_str());

    if(dir != NULL){
        while((entry = readdir(dir))){
            if (entry->d_name[0] != '.') {
                string fileName = folderName + entry->d_name;
                readTagsFromFile(fileName, graph);
            }
        }
        closedir(dir);
    } else {
        abort();
    }
}

void Converter::readTagsFromFile(const string &fileName, Graph<VertexInfoTXT, int> *graph) {
    string line;

    ifstream poiFile(fileName);

    if (poiFile.is_open()){
        getline(poiFile, line);
        unsigned tagsTypes = stoi(line);
        for (unsigned i = 0; i < tagsTypes; i++){
            getline(poiFile, line);
            string tag = line.substr(line.find('=') + 1, line.npos);
            getline(poiFile, line);
            unsigned  numberOfPOI = stoi(line);
            for (int j = 0; j < numberOfPOI; j++){
                getline(poiFile, line);
                graph->findVertex(stoi(line))->setPoi(tag);

            }
        }
    } else {
        abort();
    }

    poiFile.close();
}*/


