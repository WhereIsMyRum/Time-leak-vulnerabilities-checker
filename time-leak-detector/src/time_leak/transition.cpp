#include <string>
#include <iostream>

#include "../../include/time_leak/transition.hpp"
#include "../../include/time_leak/place.hpp"

using namespace std;
using namespace enums;

time_leak::Transition::Transition(string id, bool high, TransitionType type)
    : Element(id)
{
    this->transitionType = type;
    this->high = high;
}

TransitionType time_leak::Transition::GetTransitionType()
{
    return this->transitionType;
}

bool time_leak::Transition::IsHigh()
{
    return this->high;
}

bool time_leak::Transition::canDeduceEndTime()
{
    if (!this->IsHigh())
        return true;

    map<string, Place *>::iterator iterator;
    bool canBeDeduced = false;

    for (iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (iterator->second->IsTimeDeducible())
        {
            canBeDeduced = true;
            break;
        }
    }

    return canBeDeduced;
}

bool time_leak::Transition::canDeduceStartTime()
{
    if (!this->IsHigh())
        return true;

    map<string, Place *>::iterator iterator;
    bool canBeDeduced = false;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (iterator->second->IsTimeDeducible())
        {
            canBeDeduced = true;
            break;
        }
    }
    return canBeDeduced;
}

void time_leak::Transition::Analyze()
{
    cout << "Analyzing " << this->id << endl;
    if (this->canDeduceEndTime())
        this->transitionType = TransitionType::lowEnd;
    if (this->canDeduceStartTime())
        this->transitionType = this->transitionType == TransitionType::lowEnd ? TransitionType::low : TransitionType::lowStart;
    this->SetAnalyzed(true);
}

void time_leak::PopulateTransitions(rapidjson::Document &net)
{
    rapidjson::Value::ConstValueIterator iterator;
    for (iterator = net["transitions"]["high"].Begin(); iterator != net["transitions"]["high"].End(); ++iterator)
    {
        globals::Transitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString())));
    }

    for (iterator = net["transitions"]["low"].Begin(); iterator != net["transitions"]["low"].End(); ++iterator)
    {
        globals::Transitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString(), false, enums::TransitionType::low)));
    }
}

void time_leak::CreateTransitionsForwardLinks(rapidjson::Document &net)
{
    map<string, time_leak::Transition *>::iterator iterator;
    for (iterator = globals::Transitions.begin(); iterator != globals::Transitions.end(); ++iterator)
    {
        const char *transitionKey = iterator->second->GetId().c_str();
        for (rapidjson::SizeType i = 0; i < net["flows"]["transitions"][transitionKey].Size(); ++i)
        {
            string placeId = net["flows"]["transitions"][transitionKey][i].GetString();
            placeId = globals::Places.find(placeId) == globals::Places.end() ? "end" : placeId;
            iterator->second->AddOutElement(globals::Places.at(placeId));

            time_leak::CreatePlaceBackwardLink(placeId, iterator->second);
        }
    }
}

void time_leak::CreateTransitionBackwardLink(string transitionId, time_leak::Place *place)
{
    globals::Transitions.at(transitionId)->AddInElement(place);
}
