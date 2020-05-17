#include "StringMatcher.h"


int StringMatcher::getEditDistance(const string a, const string b) {
    int d[a.size()+1][b.size()+1];
    for (int i = 0; i <= a.size(); i++) d[i][0] = i;
    for (int j = 0; j <= b.size(); j++) d[0][j] = j;

    for (int i = 1; i <= a.size(); i++) {
        for (int j = 1; j <= b.size(); j++) {
            if (a.at(i-1) == b.at(j-1)) d[i][j] = d[i-1][j-1];
            else d[i][j] = min(d[i-1][j-1]+1, min(d[i-1][j]+1, d[i][j-1]+1));
        }
    }

    return d[a.size()][b.size()];
}
