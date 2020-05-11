#include "UserInput.h"


string UserInput::getLine(string getterPhrase) {
    string line;
    cout << getterPhrase << endl;
    getline(cin, line);
    return line;
}

float UserInput::getFloat(string getterPhrase) {
    while(true) {
        try {
            string s = getLine(getterPhrase);
            float n = stof(s);
            return n;
        } catch (...) {
            cout << "Please insert a valid number." << endl;
        }
    }
}

int UserInput::getInt(string getterPhrase) {
    while(true) {
        try {
            string s = getLine(getterPhrase);
            int n = stoi(s);
            return n;
        } catch (...) {
            cout << "Please insert a valid number." << endl;
        }
    }
}

bool UserInput::getConfirmation(string getterPhrase) {
    while (true) {
        string line = getLine(getterPhrase);
        string lowerLine = strToLower(line);
        if (lowerLine == "y" || lowerLine == "yes") {
            return true;
        } else if (lowerLine == "n" || lowerLine == "no") {
            return false;
        } else {
            cout << "Please insert a valid confirmation." << endl;
        }
    }
}

string UserInput::strToLower(string s) {
    string res;
    for (char c : s) res += tolower(c);
    return res;
}
