#include "Menu.h"
#include "../input/UserInput.h"

unsigned Menu::addOption(string opt) {
    menuOptions.push_back(opt);
    return menuOptions.size()-1;
}

void Menu::draw(string separator) const {
    drawExtraInput();
    cout << endl;
    drawMenuOptions(separator);
}

void Menu::drawMenuOptions(string separator) const {
    for (int i = 0; i < menuOptions.size(); i++) {
        if (i != 0) cout << separator;
        cout << '[' << i << "] " << menuOptions.at(i) << endl;
    }
}

int Menu::getResponse(string getterPhrase, string &res) const {
    while(true) {
        string response = UserInput::getLine(getterPhrase, false);
        try {
            int i = stoi(response);
            if (i >= 0 && i <= menuOptions.size() -1) return i;
        } catch(...) {}


        pair<string, int> r;
        r.second = INT32_MAX;
        for (const string &s : extraInput) {
            int editDistance = StringMatcher::getEditDistance(response, s);
            if (editDistance < r.second) {
                r.first = s;
                r.second = editDistance;
            }
        }

        if (r.second < 6) {
            res = r.first;
            return -1;
        }

        cout << "Insert a valid option." << endl;
    }
}

unsigned Menu::getResponse(string getterPhrase) const {
    return UserInput::getInt(getterPhrase, 0, (int) menuOptions.size() - 1, false);
}


void Menu::addExtraInput(vector<string>& input) {
    extraInput.insert(extraInput.end(), input.begin(), input.end());
}

void Menu::drawExtraInput() const {
    for (const string &s : extraInput) {
        cout << s << endl;
    }
}


