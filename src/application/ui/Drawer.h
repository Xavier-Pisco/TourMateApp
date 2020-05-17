#ifndef SRC_DRAWER_H
#define SRC_DRAWER_H

#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

class Drawer {
public:
    static void drawTitle(string title, int margin_left = 0, int width = 80, bool simple = false, string alignment = "center", char filler_outside = '*', char filler_inside = '-', ostream &os = cout);
};


#endif //SRC_DRAWER_H
