#include "Menu.h"
#include "../input/UserInput.h"

unsigned Menu::addOption(string opt) {
    menuOptions.emplace_back(opt, -1);
    return menuOptions.size()-1;
}

unsigned Menu::addOption(string opt, int n) {
    menuOptions.emplace_back(opt, n);
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
        cout << '[' << i << "] " << menuOptions.at(i).first << endl;
    }
}

int Menu::getResponse(string getterPhrase, string &res) {
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
            extraInput.erase(find(extraInput.begin(), extraInput.end(), res));
            return -1;
        }

        cout << "Insert a valid option." << endl;
    }
}

unsigned Menu::getResponse(string getterPhrase) const {
    int n = UserInput::getInt(getterPhrase, 0, (int) menuOptions.size() - 1, false);
    pair<string, int> opt = menuOptions.at(n);
    if (opt.second == -1) return n;
    else return opt.second;
}


void Menu::addExtraInput(vector<string>& input) {
    extraInput.insert(extraInput.end(), input.begin(), input.end());
}

void Menu::drawExtraInput() const {
    for (const string &s : extraInput) {
        cout << s << endl;
    }
}


