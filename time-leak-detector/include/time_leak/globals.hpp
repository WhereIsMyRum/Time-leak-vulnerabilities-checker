#include <string>
#include <map>
#include <queue>
#include <set>

#ifndef GLOBALS
#define GLOBALS

#define FILE_EXT ".json"

#include "enums.hpp"

namespace time_leak {
    class Place;
    class Transition;
}

namespace globals {
    template<class T> class UniqueFifo
    {
        private:
        std::queue<T> Queue;
        std::set<T> Set;

        public:
        void Push(T t)
        {
            if (this->Set.find(t) == this->Set.end())
            {
                this->Queue.push(t);
                this->Set.insert(t);
            }
        }

        T Pop()
        {
            T t = this->Queue.front();
            this->Queue.pop();
            return t;
        }

        int Size()
        {
            return this->Queue.size();
        }

        T Front()
        {
            return this->Queue.front();
        }
    };
}

namespace globals {
    
    extern std::map<std::string, time_leak::Place*> Places;
    extern std::map<std::string, time_leak::Transition*> Transitions;
    extern std::map<enums::TransitionType, std::string> TransitionTypeToString;
    extern UniqueFifo<time_leak::Place*> PlacesAnalyzeQueue;
    extern UniqueFifo<time_leak::Transition*> TransitionsAnalyzeQueue;
}

#endif