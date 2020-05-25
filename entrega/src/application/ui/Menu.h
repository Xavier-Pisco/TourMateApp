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
    /**
     * @brief adds an option to the menu
     * @param opt the option text
     * @return the option's number
     */
    unsigned addOption(string opt);

    /**
     * @brief adds an option with a custom number (useful for using enums to save actions)
     * @param opt - the option text
     * @param n - the option's number
     * @return the option's number
     */
    unsigned addOption(string opt, int n);

    /**
     * @brief draws the menu
     * @param separator - separates the lines
     */
    void draw(string separator = "\n") const;

    /**
     * @brief gets response from user
     * @param getterPhrase - the phrase to present to the user
     * @param res - response may be sent through res if it is a string
     * @return -1 when the response is sent through res, otherwise returns the selected option number
     */
    int getResponse(string getterPhrase, string &res);

    /**
     * @brief gets response from user
     * @param getterPhrase - the phrase to present to the user
     * @return the selected option number
     */
    unsigned getResponse(string getterPhrase) const;

    /**
     * @brief adds extra string input (sometimes you want to have the user choose from strings shown on the screen)
     * @param input - vector with the possible string
     */
    void addExtraInput(vector<string>& input);

    /**
     * @brief draws the extra input
     */
    void drawExtraInput() const;

    /**
     * @brief draws the simple menu options
     * @param separator - separates the lines
     */
    void drawMenuOptions(string separator = "\n") const;
};


#endif //SRC_MENU_H
