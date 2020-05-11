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

    double getX() {
        return x;
    }

    double  getY() {
        return y;
    }
};


#endif //SRC_POSITION_H
