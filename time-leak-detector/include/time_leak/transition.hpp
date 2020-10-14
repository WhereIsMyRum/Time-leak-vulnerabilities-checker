#ifndef TRANSITION
#define TRANSITION

#include <string>
#include <iostream>
#include <map>

#include "element.hpp"
#include "enums.hpp"

#include "../../include/time_leak/place.hpp"


class Place;

namespace time_leak
{
    class Transition : public time_leak::Element<Place>
    {
    private:
        bool high;
        enums::TransitionType transitionType;
        bool canDeduceEndTime();
        bool canDeduceStartTime();
        void analyzeIngoing();

    public:
        Transition(std::string id, bool high = true, enums::TransitionType type = enums::TransitionType::high);
        enums::TransitionType GetTransitionType();
        bool IsHigh();
        bool CheckIfLow();
        bool Analyze();
    };
} // namespace time_leak

#endif