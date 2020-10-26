#ifndef TRANSITION
#define TRANSITION

#include <string>
#include <iostream>
#include <map>

#include "element.hpp"

#include "../../include/time_leak/place.hpp"


class Place;

namespace time_leak
{
    class Transition : public time_leak::Element<Place>
    {
    public:
        enum TransitionType 
        {
            high, low, lowStart, lowEnd, maxDuration, parallel
        };

    private:
        bool highT;
        bool canDeduceEndTime();
        bool canDeduceStartTime();
        void analyzeIngoing();
        TransitionType transitionType;

    public:
        Transition(std::string id, bool high = true, TransitionType type = TransitionType::high);
        TransitionType GetTransitionType();
        bool IsHigh();
        bool CheckIfLow();
        bool Analyze();
        std::string GetTransitionTypeString();
        void SetTransitionType(TransitionType t);
        std::string conditionallyLowStartId;
        std::string conditionallyLowEndId;
    };
} // namespace time_leak

#endif