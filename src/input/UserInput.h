#ifndef SRC_USERINPUT_H
#define SRC_USERINPUT_H
#include <string>
#include <iostream>

using namespace std;

class UserInput {
public:
    /**
     * @brief gets a line
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return line
     */
    static string getLine(string getterPhrase);

    /**
     * @brief gets a float number
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return number
     */
    static float getFloat(string getterPhrase);

    /**
     * @brief gets an int number
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @param lowerLimit - the lower limit of the value
     * @param upperLimit - the upper limit of the value
     * @return number
     */
    static int getInt(string &getterPhrase, int lowerLimit = INT32_MIN, int upperLimit = INT32_MAX);

    /**
     * @brief gets confirmation from the user
     * @param getterPhrase - the phrase that is displayed to the user before him inserting the information
     * @return true if user confirms, false if user cancels
     */
    static bool getConfirmation(string getterPhrase);

    /**
     * @brief converts a string to lower case
     * @param s - the sting to convert
     * @return the converted string
     */
    static string strToLower(string s);

};


#endif //SRC_USERINPUT_H
