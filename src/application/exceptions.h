#ifndef SRC_EXCEPTIONS_H
#define SRC_EXCEPTIONS_H

#include <exception>

using namespace std;

class CancelInput : exception {};

class InvalidInput : exception {};

class VertexNotFound : exception {};


#endif //SRC_EXCEPTIONS_H
