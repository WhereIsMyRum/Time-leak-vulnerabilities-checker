#ifndef PLACE
#define PLACE

#include <string>
#include <iostream>
#include <map>
#include <iterator>

#include "element.hpp"
#include "transition.hpp"

class Transition;

namespace time_leak
{
    class Place : public time_leak::Element<Transition>
    {
    private:
        int highIn;
        int highOut;

    private:
        bool timeDeducible = false;
        bool canTimeBeDeduced();
        bool checkOutgoing();
        bool checkIngoing();
        void analyzeIngoing();
        bool checkParallelCase();

    public:
        bool parallelIn = false;
        bool parallelOut = false;
        Place(std::string id);
        bool IsTimeDeducible();
        bool Analyze();
    };

} // namespace time_leak

#endif