#ifndef ELEMENT
#define ELEMENT

#include <string>
#include <map>

#include "elementUniqueFifo.hpp"
#include "../../include/rapidjson/document.h"

namespace time_leak
{
    class Place;
    class Transition;
} // namespace time_leak


namespace time_leak
{
    template <class T>
    class Element
    {
    protected:
        std::string id;
        std::map<std::string, T *> inElements;
        std::map<std::string, T *> outElements;
        bool analyzed;

    public:
        Element<T>(std::string id);

        const std::string GetId();
        const bool WasAnalyzed();
        std::map<std::string, T *> GetInElements();
        std::map<std::string, T *> GetOutElements();
        const std::map<std::string, T *> GetElementsBasedOnDirection(bool direction);

        void AddInElement(T *element);
        void AddOutElement(T *element);
        void SetAnalyzed(bool analyzed);
        void Print();
        bool AllDirectionsAnalyzed(bool direction);
        void Traverse(std::map<std::string, T *> elements, ElementUniqueFifo<T *> &queue);
    };
} // namespace time_leak
#include "../../src/time_leak/element.tpp"
#endif