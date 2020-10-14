#ifndef NET
#define NET

#include <string>
#include <iterator>

#include "place.hpp"
#include "transition.hpp"
#include "../../include/rapidjson/document.h"


namespace time_leak
{
    class Net
    {
        private:
            rapidjson::Document net;
            map<std::string, time_leak::Place *> places;
            map<std::string, time_leak::Transition *> lowTransitions;
            map<std::string, time_leak::Transition *> highTransitions;
            ElementUniqueFifo<Place *> placesQueue;
            ElementUniqueFifo<Transition *> transitionsQueue;
            bool changed;

        private:
            void populatePlaces();
            void populateTransitions();
            void createPlacesForwardLinks();
            void createPlaceBackwardLink(string transitionId, Transition *transition);
            void createTransitionsForwardLinks(map<string, Transition *> transitions);
            void createTransitionBackwardLink(string transitionId, Place *place);

            void analyzeNet(Place *startPlace, bool upwards);

            template <class T>
            void resetAnalyzedFlag(T &Elements);

            Place *findStartPlace();
            bool wasChanged();
            void changesMade();
            void resetChanged();

        public:
            Net(rapidjson::Document net);
            void PrintNet();
            void RunAnalysis();
            void PrintResults();
    };
}

#endif