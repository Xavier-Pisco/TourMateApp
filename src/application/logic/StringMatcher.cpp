#include "StringMatcher.h"
#include <queue>


int StringMatcher::getEditDistance(const string a, const string b) {
    int d[a.size() + 1][b.size() + 1];
    for (int i = 0; i <= a.size(); i++) d[i][0] = i;
    for (int j = 0; j <= b.size(); j++) d[0][j] = j;

    for (int i = 1; i <= a.size(); i++) {
        for (int j = 1; j <= b.size(); j++) {
            if (tolower(a.at(i - 1)) == tolower(b.at(j - 1))) d[i][j] = d[i - 1][j - 1];
            else d[i][j] = min(d[i - 1][j - 1] + 1, min(d[i - 1][j] + 1, d[i][j - 1] + 1));
        }
    }
    return d[a.size()][b.size()];
}

int StringMatcher::getSubstringEditDistance(const string a, const string b) {
    unsigned res = INT32_MAX; unsigned substringSize;

    int dif = (int) b.size() - (int) a.size();

    if (dif > (int) a.size()/5) substringSize = a.size() + a.size()/5;
    else substringSize = b.size();

    for (int k = 0; k < b.size() - substringSize; k++) {
        string b2 = b.substr(k, substringSize);
        int e = getEditDistance(a, b2);
        if (e < res) res = e;
    }

    return res;
}
