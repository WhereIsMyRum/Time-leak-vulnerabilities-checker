#include "../../include/time_leak/net.hpp"

using namespace std;

time_leak::Net::Net(rapidjson::Document& net)
{
    this->net.CopyFrom(net, this->net.GetAllocator());
    this->changed = true;

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
        lowTransitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString(), false, enums::TransitionType::low)));
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

void time_leak::Net::RunAnalysis()
{
    while(this->wasChanged())
    {
        this->resetChanged();
        this->analyzeNet(places.at("end"), true);

        this->transitionsQueue.Clear();
        this->placesQueue.Clear();

        this->resetAnalyzedFlag(this->places);
        this->resetAnalyzedFlag(this->highTransitions);
        this->resetAnalyzedFlag(this->lowTransitions);

        this->analyzeNet(this->findStartPlace(), false);
    }
}

template <class T>
void time_leak::Net::resetAnalyzedFlag(T &Elements)
{
    for (auto iterator = Elements.begin(); iterator != Elements.end(); ++iterator)
    {
        iterator->second->SetAnalyzed(false);
    }
}

time_leak::Place *time_leak::Net::findStartPlace()
{
    time_leak::Place *p;

    for (auto iterator = places.begin(); iterator != places.end(); ++iterator)
    {
        if (iterator->second->GetInElements().size() == 0)
        {
            p = iterator->second;
            break;
        }
    }
    return p;
}


void time_leak::Net::analyzeNet(time_leak::Place *startPlace, bool upwards)
{
    startPlace->Analyze();
    startPlace->Traverse(startPlace->GetElementsBasedOnDirection(upwards), transitionsQueue);

    while (placesQueue.Size() > 0 || transitionsQueue.Size() > 0)
    {
        if (time_leak::ForwardQueue(transitionsQueue, placesQueue, upwards))
            changesMade();
        if (time_leak::ForwardQueue(placesQueue, transitionsQueue, upwards))
            changesMade();
    }
}

void time_leak::Net::PrintResults()
{
    map<string, time_leak::Transition *>::iterator iterator;
    for (iterator = highTransitions.begin(); iterator != highTransitions.end(); ++iterator)
    {
        cout << iterator->second->GetId() << "-" << globals::TransitionTypeToString.at(iterator->second->GetTransitionType()) << endl;
    }

    /*map<string, time_leak::Place *>::iterator iterator2;
    for (iterator2 = globals::Places.begin(); iterator2 != globals::Places.end(); ++iterator2)
    {
        if (iterator2->second->IsTimeDeducible())
            cout << iterator2->second->GetId() << " is timeDeducible." << endl;
    }*/
}

bool time_leak::Net::wasChanged()
{
    return this->changed;
}

void time_leak::Net::changesMade()
{
    this->changed = true;
}

void time_leak::Net::resetChanged()
{
    this->changed = false;
}