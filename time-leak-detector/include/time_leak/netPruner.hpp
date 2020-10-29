#ifndef NETPRUNER
#define NETPRUNER

#include "net.hpp"
#include <map>
#include <string>

namespace time_leak
{
    class NetPruner
    {
        enum pruningCase
        {
            oneToOneFront,
            oneToOneBack,
            oneToManySingle,
            manyToOneSingle,
            notPrunable
        };

        pruningCase getPruningCase(Transition *transition);
        void pruneOneToOneFront(Transition *transition, Net *net);
        void pruneOneToOneBack(Transition *transition, Net *net);
        void pruneOneToManySingle(Transition *transition, Net *net);
        void pruneManyToOneSingle(Transition *transition, Net *net);
        bool checkIfAllLow(Transition *transition);
        bool checkIfSingle(Transition *transition);

        public:
        void PruneNet(Net *net);
    };
}

#endif