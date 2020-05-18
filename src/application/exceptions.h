#ifndef SRC_EXCEPTIONS_H
#define SRC_EXCEPTIONS_H

#include <exception>

using namespace std;

class CancelInput : exception {};

class ImpossibleToReach : exception {};

class InvalidInput : exception {};


#endif //SRC_EXCEPTIONS_H
