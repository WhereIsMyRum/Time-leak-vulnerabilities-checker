#include "../../include/time_leak/net.hpp"

using namespace std;

time_leak::Net::Net(rapidjson::Document net)
{
    this->net.CopyFrom(net, this->net.GetAllocator());

    const char* startPlace = findStartPlace();
    getPlace(startPlace);
}

const char* time_leak::Net::findStartPlace()
{
    rapidjson::Value::ConstValueIterator iterator;

    for (iterator = net["places"].Begin(); iterator != net["places"].End(); ++iterator)
    {
        bool found = false;
        rapidjson::Value::MemberIterator transition;

        for (transition = net["flows"]["transitions"].MemberBegin(); transition != net["flows"]["transitions"].MemberEnd(); ++transition)
        {
            rapidjson::Value::ConstValueIterator place;
            for (place = net["flows"]["transitions"][transition->name.GetString()].Begin(); place != net["flows"]["transitions"][transition->name.GetString()].End(); ++place)
            {
                if (place->GetString() == iterator->GetString())
                {
                    found = true;
                    break;
                }
            }
        }

        if (found == false)
        {
            return iterator->GetString();
        }
    }
    return "";
}

time_leak::Place *time_leak::Net::getPlace(const char* placeId)
{
    time_leak::Place *place;
    try
    {
        place = getPlaceById(placeId);
    }
    catch (const char *e)
    {
        place = createPlace(placeId);
    }

    return place;
}

time_leak::Transition *time_leak::Net::getTransition(const char* transitionId)
{
    time_leak::Transition *transition;
    try
    {
        transition = getTransitionById(transitionId);
    }
    catch (const char *e)
    {
        transition = createTransition(transitionId);
    }

    return transition;
}

time_leak::Place *time_leak::Net::createPlace(const char* placeId)
{
    time_leak::Place *place = new time_leak::Place(placeId);

    rapidjson::Value::ConstValueIterator transition;

    const rapidjson::Value &flowsPlaces = net["flows"]["places"];

    if (flowsPlaces.HasMember(placeId))
    {
        for (transition = flowsPlaces[placeId].Begin(); transition != flowsPlaces[placeId].End(); ++transition)
        {
            time_leak::Transition *outputTransition = getTransition(transition->GetString());
            place->AddOutElement(outputTransition);
            outputTransition->AddInElement(place);
        }
        places.insert(pair<string, time_leak::Place *>(placeId, place));
    }
    else
    {
        places.insert(pair<string, time_leak::Place *>("end", place));
    }

    return place;
}

time_leak::Place *time_leak::Net::getPlaceById(const char* placeId)
{
    try
    {
        return places.at(placeId);
    }
    catch (const std::out_of_range e)
    {
        throw("Place not found");
    }
}

time_leak::Transition *time_leak::Net::createTransition(const char* transitionId)
{
    time_leak::Transition *transition;
    rapidjson::Value::ConstValueIterator iterator;
    bool high = false;
    for (iterator = net["transitions"]["high"].Begin(); iterator != net["transitions"]["high"].End(); ++iterator)
    {
        if (strcmp(iterator->GetString(), transitionId) == 0)
        {
            high = true;
        }
    }

    if (high == true)
    {
        transition = new time_leak::Transition(transitionId);
        highTransitions.insert(pair<std::string, time_leak::Transition *>(transitionId, transition));
    }
    else
    {
        transition = new time_leak::Transition(transitionId, false, time_leak::Transition::TransitionType::low);
        lowTransitions.insert(pair<std::string, time_leak::Transition *>(transitionId, transition));
    }

    rapidjson::Value::ConstValueIterator place;
    for (place = net["flows"]["transitions"][transitionId].Begin(); place != net["flows"]["transitions"][transitionId].End(); ++place)
    {
        time_leak::Place *outputPlace;
        if (net["flows"]["places"].HasMember(place->GetString()))
        {
            outputPlace = getPlace(place->GetString());
        } else 
        {
            const char* id = "end";
            outputPlace = getPlace(id);
        }
        transition->AddOutElement(outputPlace);
        outputPlace->AddInElement(transition);
    }

    return transition;
}

time_leak::Transition *time_leak::Net::getTransitionById(const char* transitionId)
{
    try
    {
        return lowTransitions.at(transitionId);
    }
    catch (const out_of_range e)
    {
        try
        {
            return highTransitions.at(transitionId);
        }
        catch (const out_of_range e2)
        {
            throw("Transition not found");
        }
    }
}

void time_leak::Net::PrintNet()
{
    cout << "Places: " << places.size() << endl;
    cout << "Low Transitions: " << lowTransitions.size() << endl;
    cout << "High Transitions: " << highTransitions.size() << endl;
    places.at("end")->Print();
}

map<std::string, time_leak::Place *> &time_leak::Net::GetPlaces()
{
    return this->places;
}

map<std::string, time_leak::Transition *> &time_leak::Net::GetLowTransitions()
{
    return this->lowTransitions;
}

map<std::string, time_leak::Transition *> &time_leak::Net::GetHighTransitions()
{
    return this->highTransitions;
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