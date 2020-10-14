#ifndef PLACE
#define PLACE

#include <string>
#include <iostream>
#include <map>
#include <iterator>

#include "element.hpp"
#include "transition.hpp"
#include "enums.hpp"

class Transition;

namespace time_leak
{
    class Place : public time_leak::Element<Transition>
    {
    private:
        bool timeDeducible = false;
        bool canTimeBeDeduced();
        bool checkOutgoing();
        bool checkIngoing();
        void analyzeIngoing();

    public:
        Place(std::string id);
        bool IsTimeDeducible();
        bool Analyze();
    };

} // namespace time_leak

#endif