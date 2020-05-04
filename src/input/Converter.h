#ifndef SRC_CONVERTER_H
#define SRC_CONVERTER_H

#include "input.cpp"

/**
 * The objective of this class is to convert a map
 * file into a graph that can be used by the Application
 */
class Converter {
private:
    static int readFileData(const string &fileDir, string &data);
    static Graph<StreetIntersection, Road> * parseXMLDocToGraph(rapidxml::xml_document<> &doc);
    static rapidxml::xml_document<> * createXMLDoc(char * data);
public:
    static Graph<StreetIntersection, Road> * getGraphFromOSMFile(string fileName);
};


#endif //SRC_CONVERTER_H
