//
// Created by xavier on 11/05/20.
//

#ifndef SRC_ROADINTERCEPTIONFROMTXT_H
#define SRC_ROADINTERCEPTIONFROMTXT_H


#include <string>
#include <iostream>
#include "Position.h"

using namespace std;

class RoadInterceptionFromTxt {
private:
    Position p = Position(0, 0);
    unsigned id;
    string poiType = "";
public:
    RoadInterceptionFromTxt(unsigned id, Position p) {
        this->id = id;
        this->p = p;
    }


    RoadInterceptionFromTxt(unsigned id) {
        this->id = id;
    }

    friend bool operator==(const RoadInterceptionFromTxt& r1, const RoadInterceptionFromTxt& r2){
        return r1.id == r2.id;
    }

    friend ostream& operator <<(ostream &os, const RoadInterceptionFromTxt &r){
        os << r.id << " - (" << r.p.getX() << ", " << r.p.getY() << ") - " << r.poiType << endl;
        return os;
    }

    void setPoi(const string& poi){
        this->poiType = poi;
    }
};


#endif //SRC_ROADINTERCEPTIONFROMTXT_H
