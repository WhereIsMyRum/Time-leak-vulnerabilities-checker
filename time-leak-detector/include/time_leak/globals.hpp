#include <string>
#include <map>
#include <queue>
#include <set>

#ifndef GLOBALS
#define GLOBALS

#define FILE_EXT ".json"

#include "enums.hpp"
#include "elementUniqueFifo.hpp"

namespace time_leak
{
    class Place;
    class Transition;
} // namespace time_leak

namespace globals
{
    extern std::map<std::string, time_leak::Place *> Places;
    extern std::map<std::string, time_leak::Transition *> Transitions;
    extern std::map<enums::TransitionType, std::string> TransitionTypeToString;
    extern time_leak::ElementUniqueFifo<time_leak::Place *> PlacesQueue;
    extern time_leak::ElementUniqueFifo<time_leak::Transition *> TransitionsQueue;
} // namespace globals

#endif