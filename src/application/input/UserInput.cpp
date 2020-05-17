#include "UserInput.h"


string UserInput::getLine(string getterPhrase, bool cancelInput) {
    string line;
    cout << getterPhrase << endl;
    getline(cin, line);
    if (cancelInput && strToLower(line) == "stop") throw CancelInput();
    return line;
}

float UserInput::getFloat(string getterPhrase, float lowerLimit, float upperLimit, bool cancelInput) {
    while(true) {
        string s = getLine(getterPhrase, cancelInput);
        try {
            float n = stof(s);
            if (n < lowerLimit || n > upperLimit) throw InvalidInput();
            return n;
        } catch (...) {
            cout << "Please insert a valid value." << endl;
        }
    }
}

double UserInput::getDouble(string getterPhrase, double lowerLimit, double upperLimit, bool cancelInput) {
    while(true) {
        string s = getLine(getterPhrase, cancelInput);
        try {
            double n = stod(s);
            if (n < lowerLimit || n > upperLimit) throw InvalidInput();
            return n;
        } catch (...) {
            cout << "Please insert a valid value." << endl;
        }
    }
}

int UserInput::getInt(string &getterPhrase, int lowerLimit, int upperLimit, bool cancelInput) {
    string s;
    while(true) {
        s = getLine(getterPhrase, cancelInput);
        try {
            int n = stoi(s);
            if (n < lowerLimit || n > upperLimit) throw InvalidInput();
            return n;
        } catch (...) {
            cout << "Please insert a valid value." << endl;
        }
    }
}

bool UserInput::getConfirmation(string getterPhrase) {
    stringstream s; s << getterPhrase << " (y/n)";
    while (true) {
        string line = getLine(s.str());
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

string UserInput::getPreference() {
    Menu menu;
    menu.addOption("Done");
    menu.addOption("Museums");
    menu.addOption("Statues");
    menu.addOption("Restaurants");
    menu.addOption("Bakery");
    menu.addOption("Cafe");
    menu.addOption("Gardens");

    menu.drawMenuOptions("");
    cout << endl;
    int opt = menu.getResponse("Choose an option from the menu:");


    switch(opt) {
        case 0:
            return "done";
        case 1:
            return "museum";
        case 2:
            return "statue";
        case 3:
            return "restaurant";
        case 4:
            return "bakery";
        case 5:
            return "cafe";
        case 6:
            return "garden";
    }

}