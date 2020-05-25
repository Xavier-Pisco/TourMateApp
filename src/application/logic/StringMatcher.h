#ifndef SRC_STRINGMATCHER_H
#define SRC_STRINGMATCHER_H

#include <string>

using namespace std;

class StringMatcher {
public:
    /**
     * @brief implements string matching algorithm, gets the edit distance between two strings
     * @param a
     * @param b
     * @return the edit distance between a and b
     */
    static int getEditDistance(const string a, const string b);

    /**
     * searches in b for the occurence in b that has the least edit distance from a
     * @param a
     * @param b
     * @return the edit distance of the closest occurence of a in b
     */
    static int getSubstringEditDistance(const string a, const string b);
};


#endif //SRC_STRINGMATCHER_H
