#ifndef SRC_CONVERTER_H
#define SRC_CONVERTER_H

#include "../graph/GraphClasses.h"
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <algorithm>

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
    static Graph<VertexInfoXML, Road> * parseXMLDocToGraph(rapidxml::xml_document<> &doc);

    /**
     * Creates a RapidXML doc from data in c string format
     * @param data - the data to be converted
     * @return pointer to the XML document
     */
    static rapidxml::xml_document<> * createXMLDoc(char * data);


    /**
     * Parses a line of nodes to a vector
     * @param line node info
     * @return
     */
    static vector<double> parseNodeLineToInts(string & line);

    /**
     * Parses a line of edges to a vector
     * @param line edge info
     * @return
     */
    static vector<double> parseEdgeLineToInts(string & line);

    /**
     * Reads nodes from a file
     * @param fileName
     * @param graph
     */
    static void readNodeFileTxt(const string& fileName, Graph<VertexInfoTXT, int> * graph);

    /**
     * Reads edges from a file
     * @param fileName
     * @param graph
     */
    static void readEdgesFileTxt(const string& fileName, Graph<VertexInfoTXT, int> * graph);

    /**
     * Reads all tags from a folder
     * @param folderName
     * @param graph
     */
    static void readTagsFromFolder(const string& folderName, Graph<VertexInfoTXT, int> * graph);

    static void readTagsFromFile(const string& fileName, Graph<VertexInfoTXT, int> * graph);
public:
    /**
     * Converts an OpenStreetMap file to graph format
     * @param fileName - name of the file to get graph from
     * @return pointer to the graph
     */
    static Graph<VertexInfoXML, Road> * getGraphFromOSMFile(const string& fileName);

    /**
     * Converts info from a city to a graph
     * @param nodesFileName
     * @param edgesFileName
     * @param poiFileName
     * @return
     */
    static Graph<VertexInfoTXT, int> * getGraphFromTXTFile(const string& city);
};


#endif //SRC_CONVERTER_H
