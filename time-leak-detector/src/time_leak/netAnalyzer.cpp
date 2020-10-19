#include "../../include/time_leak/netAnalyzer.hpp"

using namespace std;

void time_leak::NetAnalyzer::RunAnalysis(time_leak::Net &net)
{
    while(this->wasChanged())
    {
        this->resetChanged();
        this->analyzeNet(net, net.GetPlaces().at("end"), true);

        net.GetTransitionsQueue().Clear();
        net.GetPlacesQueue().Clear();

        this->resetAnalyzedFlag(net.GetPlaces());
        this->resetAnalyzedFlag(net.GetHighTransitions());
        this->resetAnalyzedFlag(net.GetLowTransitions());
        this->analyzeNet(net, this->findStartPlace(net.GetPlaces()), false);
    }
    this->checkForSpecialCases(net.GetHighTransitions());

    //printResults(net.GetHighTransitions());
}

void time_leak::NetAnalyzer::analyzeNet(time_leak::Net &net, time_leak::Place *startPlace, bool upwards)
{
    startPlace->Analyze();
    startPlace->Traverse(startPlace->GetElementsBasedOnDirection(upwards), net.GetTransitionsQueue());

    while (net.GetPlacesQueue().Size() > 0 || net.GetTransitionsQueue().Size() > 0)
    {
        if (time_leak::ForwardQueue(net.GetTransitionsQueue(), net.GetPlacesQueue(), upwards))
            changesMade();
        if (time_leak::ForwardQueue(net.GetPlacesQueue(), net.GetTransitionsQueue(), upwards))
            changesMade();
    }
}

template <class T>
void time_leak::NetAnalyzer::resetAnalyzedFlag(T &Elements)
{
    for (auto iterator = Elements.begin(); iterator != Elements.end(); ++iterator)
    {
        iterator->second->SetAnalyzed(false);
    }
}

time_leak::Place *time_leak::NetAnalyzer::findStartPlace(map<string, time_leak::Place*> places)
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

void time_leak::NetAnalyzer::checkForSpecialCases(map<string, time_leak::Transition*> &highTransitions)
{
    for (auto iterator = highTransitions.begin(); iterator != highTransitions.end(); ++iterator)
    {
        this->checkIntervalOnlyCase(iterator->second);
    }
}

void time_leak::NetAnalyzer::checkIntervalOnlyCase(time_leak::Transition *transition)
{
    if (transition->GetTransitionType() != Transition::TransitionType::low)
        return;

    map<string, time_leak::Place *> places = transition->GetOutElements();
    for (auto it1 = places.begin(); it1 != places.end(); ++it1)
    {
        map<string, time_leak::Transition *> transitions = it1->second->GetOutElements();
        for (auto it2 = transitions.begin(); it2 != transitions.end(); ++it2)
        {
            if (it2->second->CheckIfLow())
            {
                if (it2->second->GetInElements().size() > 1)
                {
                    transition->SetTransitionType(Transition::TransitionType::maxDuration);
                }
                else
                {
                    transition->SetTransitionType(Transition::TransitionType::low);
                    return;
                }
            }
        }
    }
}

bool time_leak::NetAnalyzer::wasChanged()
{
    return this->changed;
}

void time_leak::NetAnalyzer::changesMade()
{
    this->changed = true;
}

void time_leak::NetAnalyzer::resetChanged()
{
    this->changed = false;
}

void time_leak::NetAnalyzer::printResults(map<string, time_leak::Transition*> &highTransitions)
{
    map<string, time_leak::Transition *>::iterator iterator;
    for (iterator = highTransitions.begin(); iterator != highTransitions.end(); ++iterator)
    {
        cout << iterator->second->GetId() << "-" << iterator->second->GetTransitionTypeString() << endl;
    }

    /*map<string, time_leak::Place *>::iterator iterator2;
    for (iterator2 = globals::Places.begin(); iterator2 != globals::Places.end(); ++iterator2)
    {
        if (iterator2->second->IsTimeDeducible())
            cout << iterator2->second->GetId() << " is timeDeducible." << endl;
    }*/
}