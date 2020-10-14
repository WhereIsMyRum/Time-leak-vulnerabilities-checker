#include <string>
#include <map>
#include <queue>
#include <set>

#ifndef GLOBALS
#define GLOBALS

#define FILE_EXT ".json"

#include "enums.hpp"

namespace globals
{
    extern std::map<enums::TransitionType, std::string> TransitionTypeToString;
} // namespace globals

#endif