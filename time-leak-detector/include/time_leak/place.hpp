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
        bool endTimeDeducible = false;
        bool startTimeDeducible = false;
        void canTimeBeDeduced();
        void analyzeIngoing();
        bool isEndTimeDeducible();
        bool isStartTimeDeducible();
        void countHighInAndHighOut();

    public:
        Place(std::string id);
        bool GetEndTimeDeducible();
        bool GetStartTimeDeducible();
        bool Analyze();
    };

} // namespace time_leak

#endif