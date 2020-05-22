#ifndef SRC_MENU_H
#define SRC_MENU_H

#include <string>
#include <iostream>
#include <vector>

using namespace std;

class Menu {
private:
    vector<pair<string, int>> menuOptions;
    vector<string> extraInput;
public:
    unsigned addOption(string opt);
    unsigned addOption(string opt, int n);
    void draw(string separator = "\n") const;
    int getResponse(string getterPhrase, string &res) const;
    unsigned getResponse(string getterPhrase) const;
    void addExtraInput(vector<string>& input);
    void drawExtraInput() const;
    void drawMenuOptions(string separator = "\n") const;
};


#endif //SRC_MENU_H
