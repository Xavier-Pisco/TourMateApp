#ifndef SRC_POSITION_H
#define SRC_POSITION_H


class Position {
private:
    double lat, lon;
public:
    Position(double lat, double lon) {
        this->lat = lat;
        this->lon = lon;
    }

    double getLat() const {
        return lat;
    }

    double getLon() const {
        return lon;
    }
};


#endif //SRC_POSITION_H
