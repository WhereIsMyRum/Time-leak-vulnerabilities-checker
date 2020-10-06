#ifndef TRANSITION
#define TRANSITION

#include <string>
#include <map>

#include "element.hpp"
#include "enums.hpp"

class Place;

namespace time_leak {
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
        void AnalyzeTransition();
        bool IsHigh();
    };
}

#endif