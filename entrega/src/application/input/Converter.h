#ifndef SRC_CONVERTER_H
#define SRC_CONVERTER_H

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "../graph/GraphEdgeInfo.h"
#include "../ui/Menu.h"
#include "../exceptions.h"

typedef pair<long, pair<double, double>> NodeIdLatLon;
typedef pair<long, long> EdgeVertexIds;

/**
 * The objective of this class is to convert a map
 * file into a graph that can be used by the Application
 */
class Converter {
private:
    /**
     * @brief Reads file data from file into string
     * @param fileDir - the path to the file
     * @param data - the reference that will receive the data
     * @return 0 on success
     */
    static int readFileData(const string &fileDir, string &data);

    /**
     * @brief Parses an XML doc from RapidXML to graph format
     * @param doc - the XMLDoc
     * @return pointer to the graph
     */
    static Graph<VertexInfoXML> * parseXMLDocToGraph(rapidxml::xml_document<> &doc, vector<WayInfoXML*> &roads, vector<WayInfoXML*> &placesWays, vector<VertexInfoXML> &placesNodes);

    /**
     * @brief Creates a RapidXML doc from data in c string format
     * @param data - the data to be converted
     * @return pointer to the XML document
     */
    static rapidxml::xml_document<> * createXMLDoc(char * data);

    /**
     * @brief Converts a value in degrees to radians
     * @param degrees - the value to convert
     * @return the value in radians
     */
    static double degreesToRadians(double degrees);

    /**
     * @brief Gets the distance in km from two lat lon points
     * @param lat1
     * @param lat2
     * @param lon1
     * @param lon2
     * @return distance value in km
     */
    static double getKmDistfromLatLong(double lat1, double lon1, double lat2, double lon2);


    /**
     * Parses a line of nodes
     * @param line node info
     * @return {vertexId, {lat, lon}}
     */
    static NodeIdLatLon parseNodeLine(string & line);

    /**
     * Parses a line of edges
     * @param line edge info
     * @return {vertex1Id, vertex2Id}
     */
    static EdgeVertexIds parseEdgeLine(string & line);

    /**
     * Reads nodes from a file
     * @param fileName
     * @param graph
     */
    static map<long, Vertex<VertexInfoTXT>*> readNodesFileTxt(const string& fileName, Graph<VertexInfoTXT> * graph);

    /**
     * Reads edges from a file
     * @param fileName
     * @param graph
     */
    static void readEdgesFileTxt(const string& fileName, Graph<VertexInfoTXT> * graph, map<long, Vertex<VertexInfoTXT>*> &nodes, bool gridGraph);

    /**
     * Reads all tags from a folder
     * @param folderName
     * @param graph
     */
    static void readTagsFileTXT(const string& fileName, Graph<VertexInfoTXT> * graph, map<long, Vertex<VertexInfoTXT>*> &nodes);

    static void generateTagsFileTXT(const string& fileName, map<long, Vertex<VertexInfoTXT>*> &nodes);

public:
    /**
     * Converts an OpenStreetMap file to graph format
     * @param fileName - name of the file to get graph from
     * @return pointer to the graph
     */
    static Graph<VertexInfoXML> * getGraphFromOSMFile(const string& fileName, vector<WayInfoXML*> &roads, vector<WayInfoXML*> &placesWays, vector<VertexInfoXML> &placesNodes);

    /**
     * Converts info from a city to a graph
     * @param city - the name of the city
     * @param nodes - the nodes vector
     * @param performanceTesting - 0 if full component, 1 if strong component, -1 if not
     * @return the graph
     */
    static Graph<VertexInfoTXT> * getGraphFromTXTFile(const string& city, map<long, Vertex<VertexInfoTXT>*> &nodes, int performanceTesting = -1);
};


#endif //SRC_CONVERTER_H
