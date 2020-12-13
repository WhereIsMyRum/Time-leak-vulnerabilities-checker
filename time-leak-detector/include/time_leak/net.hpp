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

        private:
            const char* findStartPlace();
            time_leak::Place * getPlace(const char* placeId);
            time_leak::Transition * getTransition(const char* transitionId);
            time_leak::Place * createPlace(const char* placeId);
            time_leak::Transition * createTransition(const char* transitionId);
            time_leak::Place * getPlaceById(const char* placeId);
            time_leak::Transition * getTransitionById(const char* transitionId);

        public:
            Net(rapidjson::Document net);
            map<std::string, time_leak::Place *>& GetPlaces();
            map<std::string, time_leak::Transition *>& GetLowTransitions();
            map<std::string, time_leak::Transition *>& GetHighTransitions();
            void RemovePlace(std::string placeId);
            void RemoveTransition(std::string transitionId);
            void PrintNet();
    };
}

#endif