#include "Converter.h"

Graph<VertexInfoXML> * Converter::getGraphFromOSMFile(const string& fileName, vector<WayInfoXML*> &roads, vector<WayInfoXML*> &placesWays, vector<VertexInfoXML> &placesNodes) {
    string fileContent;
    if (readFileData(fileName, fileContent) != 0) {
        cout << "Error reading input file!" << endl;
        abort(); // this is temporary (or not)
    }
    rapidxml::xml_document<> * doc;
    doc = createXMLDoc((char*) fileContent.c_str());

    Graph<VertexInfoXML> * myGraph = parseXMLDocToGraph(*doc, roads, placesWays, placesNodes);

    return myGraph;
}


Graph<VertexInfoXML> * Converter::parseXMLDocToGraph(rapidxml::xml_document<> &doc, vector<WayInfoXML*> &roads, vector<WayInfoXML*> &placesWays, vector<VertexInfoXML> &placesNodes) {
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

    auto * res = new Graph<VertexInfoXML>;

    map<string, Vertex<VertexInfoXML> *> nodes;

    pair<double, double> minCoords, maxCoords;
    bool maxMinDone = false;
    // looping through child nodes of osm node and adding them to nodes
    for (rapidxml::xml_node<> *node = doc.last_node()->first_node(); node; node = node->next_sibling()) {
        if (!maxMinDone && strcmp(node->name(), "bounds") == 0) {
            XMLNode n(node);
            minCoords.first = stod(n.getXMLAttributes().at("minlat"));
            minCoords.second = stod(n.getXMLAttributes().at("minlon"));
            maxCoords.first = stod(n.getXMLAttributes().at("maxlat"));
            maxCoords.second = stod(n.getXMLAttributes().at("maxlon"));
            res->setMaxMinCoords(minCoords, maxCoords);
            maxMinDone = true;
            //cout << minCoords.first << " " << minCoords.second << " " << maxCoords.first << " " << maxCoords.second << endl;
        }
        if (strcmp(node->name(), "node") == 0) {
            nodes[node->first_attribute()->value()] = new Vertex<VertexInfoXML>(VertexInfoXML(node));
        }
    }

    /*
     * Now, ways may also be buildings and places
     * Need to find a way to save those ways too.
     * When those are referenced, we need to find the road vertex that's nearest to
     *
     * */
    // looping though ways, incrementing VertexInfoXML count and adding roads
    for (rapidxml::xml_node<> *node = doc.last_node()->first_node(); node; node = node->next_sibling()) {
        if (strcmp(node->name(), "way") == 0) {
            WayInfoXML * candidate = new WayInfoXML(node, nodes);
            if (!candidate->getXMLTagValue("highway").empty()) {
                roads.push_back(candidate);
                for (auto v : roads.at(roads.size()-1)->getVertexes()) {
                    v->getInfo().incrementCount();
                }
            } else {
                placesWays.push_back(candidate);
                /*this for is in case we need to keep these vertexes. Right now I don't se a reason to keep them.
                 * for (auto v : roads.at(roads.size()-1)->getVertexes()) {
                    v->getInfo().incrementCount();
                }*/
            }
        }
    }


    // Erasing nodes with count == 0
    vector<string> k;
    for (auto & node : nodes) {
        if (node.second->getInfo().getCount() == 0) {
            if (!node.second->getInfo().getXMLTagValue("name").empty()) {
                placesNodes.push_back(node.second->getInfo());
            } else {
                delete node.second;
                k.push_back(node.first);
            }
        }
    }
    for (const string& key : k) {
        nodes.erase(key);
    }

    for (auto & node : nodes) {
        res->addVertex(node.second);
    }

    for (WayInfoXML * edge : roads) {
        const vector<string>& nodeIDs = edge->getNodeIDs();

        // Testing if oneway or not
        bool oneway = false;
        map<string, string>::const_iterator it;
        if ( (it = edge->getXMLTags().find("highway")) != edge->getXMLTags().end()) {
            string highwayValue = it->second;

            if ( (it = edge->getXMLTags().find("oneway")) != edge->getXMLTags().end()) {
                if (it->second == "yes" || it->second == "1" || it->second == "true") {
                    oneway = true;
                }
            } else if (highwayValue == "motorway" || highwayValue == "motorway_link" || highwayValue == "trunk_link" ||
                       highwayValue == "primary_link") {
                oneway = true;
            } else if ( (it = edge->getXMLTags().find("junction")) != edge->getXMLTags().end()) {
                if (it->second == "roundabout") {
                    oneway = true;
                }
            }
        } else {
            oneway = true;
        }

        // simplest case
        double length;
        map<string, Vertex<VertexInfoXML>*>::iterator n1, n2;
        if (nodeIDs.size() == 2) {
            if ((n1 = nodes.find(nodeIDs.at(0))) != nodes.end() && (n2 = nodes.find(nodeIDs.at(1))) != nodes.end()) {
                length = getKmDistfromLatLong(n1->second->getInfo().getLat(), n1->second->getInfo().getLon(), n2->second->getInfo().getLat(), n2->second->getInfo().getLon());

                auto e = new EdgeInfoXML<VertexInfoXML>(n2->second, length, edge);
                res->addEdge(n1->second, e);
                if (!oneway) {
                    e = new EdgeInfoXML<VertexInfoXML>(n1->second, length, edge);
                    res->addEdge(n2->second, e);
                }
            }
        } else {
            for (unsigned j = 0; j < nodeIDs.size()-1; j++) {
                if ((n1 = nodes.find(nodeIDs.at(j))) != nodes.end() && (n2 = nodes.find(nodeIDs.at(j+1))) != nodes.end()) {
                    length = getKmDistfromLatLong(n1->second->getInfo().getLat(), n1->second->getInfo().getLon(), n2->second->getInfo().getLat(), n2->second->getInfo().getLon());

                    auto e = new EdgeInfoXML<VertexInfoXML>(n2->second, length, edge);
                    res->addEdge(n1->second, e);
                    if (!oneway) {
                        e = new EdgeInfoXML<VertexInfoXML>(n1->second, length, edge);
                        res->addEdge(n2->second, e);
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

    return c * (double) earthRadius;
}


Graph<VertexInfoTXT> *Converter::getGraphFromTXTFile(const string & city, map<long, Vertex<VertexInfoTXT>*> &nodes) {
    auto graph = new Graph<VertexInfoTXT>;

    string lowerCaseCity = city;
    transform(lowerCaseCity.begin(), lowerCaseCity.end(), lowerCaseCity.begin(), ::tolower);

    string nodesFileName = "../../cal-mapas-fornecidos/PortugalMaps/" + city + "/nodes_lat_lon_" + lowerCaseCity + ".txt";
    string edgesFileName = "../../cal-mapas-fornecidos/PortugalMaps/" + city + "/edges_" + lowerCaseCity + ".txt";
    string poiFolderName = "../../cal-mapas-fornecidos/TagExamples/" + city + "/";

    nodes = readNodeFileTxt(nodesFileName, graph);

    readEdgesFileTxt(edgesFileName, graph, nodes);

    readTagsFromFolder(poiFolderName, graph, nodes);

    return graph;
}

NodeIdLatLon Converter::parseNodeLine(string &line) {
    NodeIdLatLon v;
    line.erase(0, 1);
    v.first = stol(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 1);
    v.second.first = stod(line.substr(0, line.find(','))); // lat
    line.erase(0, line.find(',') + 1);
    v.second.second = stod(line.substr(0, line.find(')'))); // lon
    return v;
}

EdgeVertexIds Converter::parseEdgeLine(string &line) {
    EdgeVertexIds v;
    line.erase(0, 1);
    v.first = stol(line.substr(0, line.find(',')));
    line.erase(0, line.find(',') + 1);
    v.second = stol(line.substr(0, line.find(',')));
    return v;
}

map<long, Vertex<VertexInfoTXT>*> Converter::readNodeFileTxt(const string &fileName, Graph<VertexInfoTXT> *graph) {
    string line;
    ifstream nodesFile(fileName);

    map<long, Vertex<VertexInfoTXT>*> nodes;

    if (nodesFile.is_open()){
        getline(nodesFile, line);
        unsigned nodes_number = stoi(line);
        while (getline(nodesFile, line)) {
            NodeIdLatLon v = parseNodeLine(line);
            long id = v.first;
            Position p = Position(v.second.first, v.second.second);

            auto vx = new Vertex<VertexInfoTXT>(VertexInfoTXT(id, p));

            nodes[vx->getInfo().getID()] = vx;
            graph->addVertex(vx);
        }
    } else {
        abort();
    }

    nodesFile.close();
    return nodes;
}

void Converter::readEdgesFileTxt(const string &fileName, Graph<VertexInfoTXT> *graph, map<long, Vertex<VertexInfoTXT>*> &nodes) {
    string line;
    ifstream edgesFile(fileName);

    if (edgesFile.is_open()){
        getline(edgesFile, line);
        unsigned edges_number = stoi(line);
        while(getline(edgesFile, line)){
            EdgeVertexIds v = parseEdgeLine(line);
            Vertex<VertexInfoTXT> *v1 = nodes.at(v.first);
            Vertex<VertexInfoTXT> *v2 = nodes.at(v.second);
            graph->addEdge(v1, new Edge<VertexInfoTXT>(v1, getKmDistfromLatLong(v1->getInfo().getLat(), v1->getInfo().getLon(), v2->getInfo().getLat(), v2->getInfo().getLon())));
        }
    } else {
        abort();
    }

    edgesFile.close();
}

void Converter::readTagsFromFolder(const string &folderName, Graph<VertexInfoTXT> *graph, map<long, Vertex<VertexInfoTXT>*> &nodes) {
    DIR *dir;
    struct dirent *entry;

    dir = opendir(folderName.c_str());

    if(dir != NULL){
        while((entry = readdir(dir))){
            if (entry->d_name[0] != '.') {
                string fileName = folderName + entry->d_name;
                readTagsFromFile(fileName, graph, nodes);
            }
        }
        closedir(dir);
    } else {
        abort();
    }
}

void Converter::readTagsFromFile(const string &fileName, Graph<VertexInfoTXT> *graph, map<long, Vertex<VertexInfoTXT>*> &nodes) {
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
                nodes.at(stol(line))->getInfo().setCategory(tag);
            }
        }
    } else {
        abort();
    }

    poiFile.close();
}


