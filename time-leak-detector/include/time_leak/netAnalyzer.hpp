#ifndef NETANALYZER
#define NETANALYZER

#include "net.hpp"

namespace time_leak
{
    class NetAnalyzer
    {
        bool changed = true;
        void analyzeNet(Net &net, Place *startPlace, bool upwards);
        void checkForSpecialCases(map<std::string, Transition*> &highTransitons); 
        void checkIntervalOnlyCase(Transition *transition);
        void checkConditionalCase(Transition *transition);
        void checkConditionallyLowStart(Transition *transition);
        void checkConditionallyLowEnd(Transition *transition);

        template <class T>
        void resetAnalyzedFlag(T &Elements);

        Place *findStartPlace(map<string, Place*> places);
        bool wasChanged();
        void changesMade();
        void resetChanged();
        void printResults(map<std::string, Transition*> &highTransitions);

        public:
            void RunAnalysis(Net &net);
    };
}

#endif