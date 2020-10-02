#ifndef ELEMENT
#define ELEMENT

#include <string>
#include <map>

#include "globals.h"

namespace time_leak {
    template <class T> class Element
    {
        std::string id;
        std::map<std::string, T*> inElements;
        std::map<std::string, T*> outElements;

        public:
        Element<T>(std::string id);
        std::string GetId();
    };
}

#include "../../src/time_leak/element.tpp"
#endif