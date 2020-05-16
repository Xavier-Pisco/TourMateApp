#ifndef SRC_POSITION_H
#define SRC_POSITION_H


class Position {
private:
    double x, y;
public:
    Position(double x,double y) {
        this->x = x;
        this->y = y;
    }

    double getX() const {
        return x;
    }

    double getY() const {
        return y;
    }
};


#endif //SRC_POSITION_H
