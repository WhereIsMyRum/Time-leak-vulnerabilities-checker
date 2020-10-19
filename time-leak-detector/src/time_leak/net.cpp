#include "../../include/time_leak/net.hpp"

using namespace std;

time_leak::Net::Net(rapidjson::Document net)
{
    this->net.CopyFrom(net, this->net.GetAllocator());

    populatePlaces();
    populateTransitions();
    createPlacesForwardLinks();
    createTransitionsForwardLinks(lowTransitions);
    createTransitionsForwardLinks(highTransitions);
}

void time_leak::Net::populatePlaces()
{
    rapidjson::Value::ConstValueIterator iterator;

    for (iterator = net["places"].Begin(); iterator != net["places"].End(); ++ iterator)
    {
        if (net["flows"]["places"].HasMember(iterator->GetString()))
        {
            places.insert(pair<string, time_leak::Place *>(iterator->GetString(), new time_leak::Place(iterator->GetString())));
        }
        else
        {
            places.insert(pair<string, time_leak::Place *>("end", new time_leak::Place(iterator->GetString())));
            places.at("end")->SetAnalyzed(true);
        }
    }
}

void time_leak::Net::populateTransitions()
{
    rapidjson::Value::ConstValueIterator iterator;
    for (iterator = net["transitions"]["high"].Begin(); iterator != net["transitions"]["high"].End(); ++iterator)
    {
        highTransitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString())));
    }

    for (iterator = net["transitions"]["low"].Begin(); iterator != net["transitions"]["low"].End(); ++iterator)
    {
        lowTransitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString(), false, Transition::TransitionType::low)));
    }
}

void time_leak::Net::createPlacesForwardLinks()
{
    map<string, time_leak::Place *>::iterator iterator;
    for (iterator = places.begin(); iterator != places.end(); ++iterator)
    {
        const char *placeKey;
        placeKey = iterator->second->GetId().c_str();
        if (net["flows"]["places"].HasMember(placeKey))
        {
            for (rapidjson::SizeType i = 0; i < net["flows"]["places"][placeKey].Size(); ++i)
            {
                string transitionId = net["flows"]["places"][placeKey][i].GetString();
                if (lowTransitions.find(transitionId) != lowTransitions.end())
                    iterator->second->AddOutElement(lowTransitions.at(transitionId));
                else
                    iterator->second->AddOutElement(highTransitions.at(transitionId));

                createTransitionBackwardLink(transitionId, iterator->second);
            }
        }
    }
}

void time_leak::Net::createPlaceBackwardLink(string transitionId, time_leak::Transition *transition)
{
    places.at(transitionId)->AddInElement(transition);
}

void time_leak::Net::createTransitionsForwardLinks(map<string, time_leak::Transition *> transitions)
{
    map<string, time_leak::Transition *>::iterator iterator;
    for (iterator = transitions.begin(); iterator != transitions.end(); ++iterator)
    {
        const char *transitionKey = iterator->second->GetId().c_str();
        for (rapidjson::SizeType i = 0; i < net["flows"]["transitions"][transitionKey].Size(); ++i)
        {
            string placeId = net["flows"]["transitions"][transitionKey][i].GetString();
            placeId = places.find(placeId) == places.end() ? "end" : placeId;
            iterator->second->AddOutElement(places.at(placeId));

            createPlaceBackwardLink(placeId, iterator->second);
        }
    }
}

void time_leak::Net::createTransitionBackwardLink(string transitionId, time_leak::Place *place)
{
    if (lowTransitions.find(transitionId) != lowTransitions.end())
        lowTransitions.at(transitionId)->AddInElement(place);
    else
        highTransitions.at(transitionId)->AddInElement(place);
}

void time_leak::Net::PrintNet()
{
    cout << "Places: " << places.size() << endl;
    cout << "Low Transitions: " << lowTransitions.size() << endl;
    cout << "High Transitions: " << highTransitions.size() << endl;
    places.at("end")->Print();
}

map<std::string, time_leak::Place *>& time_leak::Net::GetPlaces()
{
    return this->places;
}

map<std::string, time_leak::Transition *>& time_leak::Net::GetLowTransitions()
{
    return this->lowTransitions;
}

map<std::string, time_leak::Transition *>& time_leak::Net::GetHighTransitions()
{
    return this->highTransitions;
}

time_leak::ElementUniqueFifo<time_leak::Place *>& time_leak::Net::GetPlacesQueue()
{
    return this->placesQueue;
}

time_leak::ElementUniqueFifo<time_leak::Transition *>& time_leak::Net::GetTransitionsQueue()
{
    return this->transitionsQueue;
}

void time_leak::Net::RemovePlace(string placeId)
{
    this->places.erase(placeId);
}

void time_leak::Net::RemoveTransition(string transitionId)
{
    if (this->lowTransitions.find(transitionId) != this->lowTransitions.end())
    {
        this->lowTransitions.erase(transitionId);
    }
    else
    {
        this->highTransitions.erase(transitionId);
    }
};