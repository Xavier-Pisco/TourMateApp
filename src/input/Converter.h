#ifndef SRC_CONVERTER_H
#define SRC_CONVERTER_H

#include "../graph/GraphClasses.h"

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
    static Graph<StreetIntersection, Road> * parseXMLDocToGraph(rapidxml::xml_document<> &doc);

    /**
     * Creates a RapidXML doc from data in c string format
     * @param data - the data to be converted
     * @return pointer to the XML document
     */
    static rapidxml::xml_document<> * createXMLDoc(char * data);
public:
    /**
     * Converts an OpenStreetMap file to graph format
     * @param fileName - name of the file to get graph from
     * @return pointer to the graph
     */
    static Graph<StreetIntersection, Road> * getGraphFromOSMFile(const string& fileName);
};


#endif //SRC_CONVERTER_H
