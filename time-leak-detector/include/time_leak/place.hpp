#ifndef PLACE
#define PLACE

#include <string>
#include <map>

#include "element.hpp"
#include "enums.hpp"

class Transition;

namespace time_leak {
    class Place : public time_leak::Element<Transition>
    {
        private:
        bool timeDeducible = false;
        bool canTimeBeDeducted();
        bool checkOutgoing();
        bool checkIngoing();
        void analyzeIngoing();


        public:
        Place(std::string id);
        void AnalyzePlace();
        bool IsTimeDeducible();
    };
}

#endif