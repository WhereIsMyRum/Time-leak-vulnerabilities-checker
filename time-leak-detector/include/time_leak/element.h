#ifndef ELEMENT
#define ELEMENT

#include <string>
#include <map>

#include "globals.h"
#include "../../include/rapidjson/document.h"

namespace time_leak {
    template <class T> class Element
    {
        public:
        std::string id;
        std::map<std::string, T*> inElements;
        std::map<std::string, T*> outElements;

        public:
        Element<T>(std::string id);
        std::string GetId();
        void AddInElement(T *element);
        void AddOutElement(T *element);
    };
}
#include "../../src/time_leak/element.tpp"
#endif