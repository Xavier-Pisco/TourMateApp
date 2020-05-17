#include "Drawer.h"

void Drawer::drawTitle(string title, int margin_left, int width, bool simple, string alignment, char filler_outside, char filler_inside, ostream &os) {
    string top_bot, side_left, side_right;
    if (!simple && ((title.length() % 2 != 0 && width % 2 == 0) || (title.length() % 2 == 0 && width % 1 != 0))) {
        width += 1;
    }

    if (alignment == "center") {
        for (unsigned int i = 0; i < (width - title.length() - 2) / 2; i++) {
            side_left += filler_inside;
        }
        side_right = side_left;
    } else if (alignment == "left" || alignment == "right") {
        for (unsigned int i = 0; i < (width - title.length() - 2)/2 - (width - title.length() - 2)/3; i++) {
            side_left += filler_inside;
        }
        for (unsigned int i = 0; i < (width - title.length() - 2)/2 + (width - title.length() - 2)/3; i++) {
            side_right += filler_inside;
        }
    }

    if (alignment == "right") side_left.swap(side_right);

    if (simple) {
        os << setw(margin_left) << "" << side_left << " " << title << " " << side_right << endl;
    } else {
        for (int i = 0; i < width; i++) {
            top_bot += filler_outside;
        }
        os << setw(margin_left) << "" << top_bot << endl << setw(margin_left) << "" << side_left << " " << title << " " << side_right << endl << setw(margin_left) << "" << top_bot << endl;
    }
}
