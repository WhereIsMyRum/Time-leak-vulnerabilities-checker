#ifndef PLACE
#define PLACE

#include <string>
#include <map>

#include "element.h"

class Transition;

namespace time_leak {
    class Place : public time_leak::Element<Transition>
    {
        public:
        Place(std::string id);
    };
}

#endif