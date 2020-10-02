#ifndef TRANSITION
#define TRANSITION

#include <string>
#include <map>

#include "element.h"

class Place;

namespace time_leak {
    class Transition : public time_leak::Element<Place> 
    {
        public:
        Transition(std::string id);
    };
}

#endif