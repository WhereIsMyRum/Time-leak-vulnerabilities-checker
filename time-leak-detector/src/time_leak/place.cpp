#include <string>
#include <iostream>
#include <map>
#include <iterator>

#include "../../include/time_leak/place.hpp"
#include "../../include/time_leak/transition.hpp"

using namespace std;
using namespace enums;

time_leak::Place::Place(string id)
    : Element(id)
{
}

bool time_leak::Place::IsTimeDeducible()
{
    return this->timeDeducible;
}

bool time_leak::Place::canTimeBeDeduced()
{
    if (checkOutgoing() || checkIngoing())
    {
        return true;
    }
    return false;
}

bool time_leak::Place::checkOutgoing()
{
    map<string, Transition *>::iterator iterator;
    bool canBeDeduced = false;

    for (iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (!iterator->second->IsHigh() || iterator->second->GetTransitionType() == TransitionType::low || iterator->second->GetTransitionType() == TransitionType::lowStart)
        {
            canBeDeduced = true;
            break;
        }
    }

    return canBeDeduced;
}

bool time_leak::Place::checkIngoing()
{
    map<string, Transition *>::iterator iterator;
    bool canBeDeduced = this->inElements.size() > 0 ? true : false;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (iterator->second->IsHigh() && iterator->second->GetTransitionType() != TransitionType::low && iterator->second->GetTransitionType() != TransitionType::lowEnd)
        {
            canBeDeduced = false;
            break;
        }
    }
    return canBeDeduced;
}

void time_leak::Place::Analyze()
{
    //cout << "Analyzing " << this->id << endl;
    if (this->canTimeBeDeduced())
        this->timeDeducible = true;
    this->SetAnalyzed(true);
}

void time_leak::PopulatePlaces(rapidjson::Document &net)
{
    rapidjson::Value::ConstValueIterator iterator;
    for (iterator = net["places"].Begin(); iterator != net["places"].End(); ++iterator)
    {
        if (net["flows"]["places"].HasMember(iterator->GetString()))
        {
            globals::Places.insert(pair<string, time_leak::Place *>(iterator->GetString(), new time_leak::Place(iterator->GetString())));
        }
        else
        {
            globals::Places.insert(pair<string, time_leak::Place *>("end", new time_leak::Place(iterator->GetString())));
            globals::Places.at("end")->SetAnalyzed(true);
        }
    }
}

time_leak::Place *time_leak::FindStartPlace()
{
    map<string, time_leak::Place *>::iterator iterator;
    time_leak::Place *p;

    for (iterator = globals::Places.begin(); iterator != globals::Places.end(); ++iterator)
    {
        if (iterator->second->GetInElements().size() == 0)
        {
            p = iterator->second;
            break;
        }
    }

    return p;
}

void time_leak::CreatePlacesForwardLinks(rapidjson::Document &net)
{
    map<string, time_leak::Place *>::iterator iterator;
    for (iterator = globals::Places.begin(); iterator != globals::Places.end(); ++iterator)
    {
        const char *placeKey;
        placeKey = iterator->second->GetId().c_str();
        if (net["flows"]["places"].HasMember(placeKey))
        {
            for (rapidjson::SizeType i = 0; i < net["flows"]["places"][placeKey].Size(); ++i)
            {
                string transitionId = net["flows"]["places"][placeKey][i].GetString();
                iterator->second->AddOutElement(globals::Transitions.at(transitionId));

                time_leak::CreateTransitionBackwardLink(transitionId, iterator->second);
            }
        }
    }
}

void time_leak::CreatePlaceBackwardLink(string transitionId, time_leak::Transition *transition)
{
    globals::Places.at(transitionId)->AddInElement(transition);
}
