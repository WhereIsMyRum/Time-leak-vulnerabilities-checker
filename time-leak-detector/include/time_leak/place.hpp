#ifndef PLACE
#define PLACE

#include <string>
#include <map>

#include "element.hpp"
#include "enums.hpp"

class Transition;

namespace time_leak
{
    class Place : public time_leak::Element<Transition>
    {
    private:
        bool timeDeducible = false;
        bool canTimeBeDeduced();
        bool checkOutgoing();
        bool checkIngoing();
        void analyzeIngoing();

    public:
        Place(std::string id);
        void AnalyzeFirstLevel();
        void AnalyzeDeeper();
        bool IsTimeDeducible();
        void Analyze();
    };

    void PopulatePlaces(rapidjson::Document &net);
    time_leak::Place *FindStartPlace();
    void CreatePlacesForwardLinks(rapidjson::Document &net);
    void CreatePlaceBackwardLink(string transitionId, time_leak::Transition *transition);
} // namespace time_leak

#endif