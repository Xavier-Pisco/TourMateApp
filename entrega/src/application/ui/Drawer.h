#ifndef SRC_DRAWER_H
#define SRC_DRAWER_H

#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

class Drawer {
public:
    /**
     * @brief Draws a custom title
     * @param title - the text
     * @param margin_left
     * @param width - maximum width
     * @param simple - simple only takes 1 line, otherwise takes 3
     * @param alignment - alignment of the text (right, left or center)
     * @param filler_outside - filler to be user as border
     * @param filler_inside - filler to be used as background
     * @param os - outstream to be written to
     */
    static void drawTitle(string title, int margin_left = 0, int width = 80, bool simple = false, string alignment = "center", char filler_outside = '*', char filler_inside = '-', ostream &os = cout);
};


#endif //SRC_DRAWER_H
