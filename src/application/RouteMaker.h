#ifndef SRC_ROUTEMAKER_H
#define SRC_ROUTEMAKER_H

#include <string>
#include <vector>

using namespace std;

class RouteMaker {
private:

public:
    void start();
    void openMap(string &map);
    void getRouteInfo();
    vector<string> getAvailableMaps() const;
};


#endif //SRC_ROUTEMAKER_H
