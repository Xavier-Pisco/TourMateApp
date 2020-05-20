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
    unsigned opt = menu.getResponse("Choose an option from the menu:");


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
        default:
            return "";
    }

}


Vertex<VertexInfoXML> * UserInput::getVertex(MapContainer<VertexInfoXML> * mapContainer, bool mandatory) {
    Menu menu;
    menu.addOption("cancel");
    menu.addOption("add location with GPS coordinates");
    menu.addOption("add location with location name");
    if (!mandatory) menu.addOption("I do not need to specify");

    menu.drawMenuOptions("");
    cout << endl;
    unsigned opt = menu.getResponse("Choose an option from the menu:");


    switch(opt) {
        case 0:
            throw CancelInput();
        case 1:
            return UserInput::getVertexWithGPSCoords(mapContainer);
        case 2:
            return UserInput::getVertexWithLocationName(mapContainer);
        case 3:
        default:
            break;
    }
    return nullptr;
}


Vertex<VertexInfoXML> * UserInput::getVertexWithGPSCoords(MapContainer<VertexInfoXML> * mapContainer) {
    Coords coords;

    cout << "This option finds the vertex with the coordinates \nthat are the closest to the ones you specify" << endl << endl;

    coords.first = UserInput::getDouble("Latitude:");
    coords.second = UserInput::getDouble("Longitude:");

    return mapContainer->getVertexWithCoords(coords);
}


Vertex<VertexInfoXML> * UserInput::getVertexWithLocationName(MapContainer<VertexInfoXML> * mapContainer) {
    string name;
    vector<VertexNameEditDist> v;

    while (true) {
        cout << "This option finds the vertex that corresponds to the location" << endl << "that has the name closest the the one you specified."
             << endl << endl << "Insert 'stop' at any time to cancel operation." << endl << endl;


        name = UserInput::getLine("Location name:");

        v = ((OSMapContainer*) mapContainer)->getPlacePossibilitiesWithName(name);

        for (auto & it : v) {
            cout << "Found place with name " << it.second.second << endl;
        }

        cout << endl;
        stringstream s; s << "Is '" << v.at(0).second.second << "' the place you're looking for?";

        if (UserInput::getConfirmation(s.str())) break;
        v.erase(v.begin(), v.end());
    }

    return v.at(0).second.first;
}

