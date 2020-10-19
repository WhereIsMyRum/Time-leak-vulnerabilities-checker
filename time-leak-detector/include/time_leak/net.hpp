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

        private:
            void populatePlaces();
            void populateTransitions();
            void createPlacesForwardLinks();
            void createPlaceBackwardLink(string transitionId, Transition *transition);
            void createTransitionsForwardLinks(map<string, Transition *> transitions);
            void createTransitionBackwardLink(string transitionId, Place *place);

        public:
            Net(rapidjson::Document net);
            map<std::string, time_leak::Place *>& GetPlaces();
            map<std::string, time_leak::Transition *>& GetLowTransitions();
            map<std::string, time_leak::Transition *>& GetHighTransitions();
            ElementUniqueFifo<Place *>& GetPlacesQueue();
            ElementUniqueFifo<Transition *>& GetTransitionsQueue();
            void RemovePlace(std::string placeId);
            void RemoveTransition(std::string transitionId);
            void PrintNet();
    };
}

#endif