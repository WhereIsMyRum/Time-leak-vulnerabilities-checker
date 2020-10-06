#ifndef ELEMENT
#define ELEMENT

#include <string>
#include <map>

#include "globals.hpp"
#include "../../include/rapidjson/document.h"

namespace time_leak {
    template <class T> class Element
    {
        protected:
        std::string id;
        std::map<std::string, T*> inElements;
        std::map<std::string, T*> outElements;
        bool analyzed;

        public:
        Element<T>(std::string id);
        std::string GetId();
        void AddInElement(T *element);
        void AddOutElement(T *element);
        void Print();
        bool WasAnalyzed();
        void SetAnalyzed(bool analyzed);
        int GetInElementsSize();
    };
}
#include "../../src/time_leak/element.tpp"
#endif