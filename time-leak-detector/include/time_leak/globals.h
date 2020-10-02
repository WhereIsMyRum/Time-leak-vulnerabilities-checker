#include <string>
#include <map>

#ifndef GLOBALS
#define GLOBALS

#define FILE_EXT ".json"

namespace time_leak {
    class Place;
    class Transition;
}

namespace globals {
    extern std::map<std::string, time_leak::Place*> Places;
    extern std::map<std::string, time_leak::Transition*> Transitions;
}

#endif