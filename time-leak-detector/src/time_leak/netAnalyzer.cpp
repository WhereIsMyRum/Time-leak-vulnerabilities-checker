#include "../../include/time_leak/netAnalyzer.hpp"

using namespace std;

void time_leak::NetAnalyzer::RunAnalysis(time_leak::Net &net, bool runConditional)
{
    while(this->wasChanged())
    {
        this->resetChanged();
        this->analyzeNet(net, net.GetPlaces().at("end"), true);

        this->transitionsQueue.Clear();
        this->placesQueue.Clear();

        if (!this->wasChanged())
            break;

        this->resetAnalyzedFlag(net.GetPlaces());
        this->resetAnalyzedFlag(net.GetHighTransitions());
        this->resetAnalyzedFlag(net.GetLowTransitions());
        this->analyzeNet(net, this->findStartPlace(net.GetPlaces()), false);
    }
    this->checkForSpecialCases(net.GetHighTransitions(), runConditional);

    //printResults(net.GetHighTransitions());
}

void time_leak::NetAnalyzer::analyzeNet(time_leak::Net &net, time_leak::Place *startPlace, bool upwards)
{
    startPlace->Analyze();
    Traverse(startPlace->GetElementsBasedOnDirection(upwards), this->transitionsQueue);

    while (this->placesQueue.Size() > 0 || this->transitionsQueue.Size() > 0)
    {
        if (time_leak::ForwardQueue(this->transitionsQueue, this->placesQueue, upwards))
            changesMade();
        if (time_leak::ForwardQueue(this->placesQueue, this->transitionsQueue, upwards))
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

void time_leak::NetAnalyzer::checkForSpecialCases(map<string, time_leak::Transition*> &highTransitions, bool runConditional)
{
    for (auto iterator = highTransitions.begin(); iterator != highTransitions.end(); ++iterator)
    {
        this->checkIntervalOnlyCase(iterator->second);
        if (runConditional)
            this->checkConditionalCase(iterator->second);
    }

}

void time_leak::NetAnalyzer::checkIntervalOnlyCase(time_leak::Transition *transition)
{
    if (transition->GetTransitionType() != Transition::TransitionType::low && transition->GetTransitionType() != Transition::TransitionType::lowEnd)
        return;

    Transition::TransitionType initialTransitionType = transition->GetTransitionType();

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
                    if(transition->GetTransitionType() == Transition::TransitionType::lowEnd) {
                        transition->SetTransitionType(Transition::TransitionType::high);
                        return;
                    }

                    map<string, time_leak::Place *> places2 = it2->second->GetInElements();
                    for (auto it3 = places2.begin(); it3 != places2.end(); ++it3)
                    {
                        if (places.find(it3->second->GetId()) == places.end())
                        {
                            if (transition->GetTransitionType() == Transition::TransitionType::low)
                            {
                                transition->SetTransitionType(Transition::TransitionType::maxDuration);
                            }

                            if (transition->GetTransitionType() == Transition::TransitionType::lowEnd)
                            {
                                map<string, time_leak::Transition *> transitions2 = it3->second->GetInElements();
                                for (auto it4 = transitions2.begin(); it4 != transitions2.end(); ++it4)
                                {
                                    if (!it4->second->CheckIfLow())
                                    {
                                        transition->SetTransitionType(Transition::TransitionType::high);
                                    }
                                }
                            }
                        }
                    }
                }
                else
                {
                    transition->SetTransitionType(initialTransitionType);
                    return;
                }
            }
        }
    }
}

void time_leak::NetAnalyzer::checkConditionalCase(time_leak::Transition *transition)
{
    if (transition->GetTransitionType() == Transition::TransitionType::low || transition->GetTransitionType() == Transition::TransitionType::maxDuration)
        return;

    if (transition->GetTransitionType() != Transition::TransitionType::lowStart)
        this->checkConditionallyLowStart(transition);
    
    if (transition->GetTransitionType() != Transition::TransitionType::lowEnd && transition->GetTransitionType() != Transition::TransitionType::low)
        this->checkConditionallyLowEnd(transition);

    return;
}

void time_leak::NetAnalyzer::checkConditionallyLowStart(time_leak::Transition *transition)
{
    map<string, time_leak::Place*> inputPlaces = transition->GetInElements();
    bool conditionallyLowStart = true;
    bool conditionally = false;

    for (auto inputPlace = inputPlaces.begin(); inputPlace != inputPlaces.end(); ++inputPlace)
    {
        map<string, time_leak::Transition *> inputTransitions = inputPlace->second->GetInElements();
        for (auto inputTransition = inputTransitions.begin(); inputTransition != inputTransitions.end(); ++inputTransition)
        {
            if (transition->GetTransitionType() == Transition::TransitionType::high && (inputTransition->second->CheckIfLow() || inputTransition->second->GetTransitionType() == Transition::TransitionType::lowEnd) && !inputTransition->second->GetConditional())
            {
                transition->SetTransitionType(Transition::TransitionType::lowStart);
                transition->SetConditional();
                return;
            }
            else if (inputTransition->second->CheckIfLow() && inputPlace->second->GetHighOut() < 2 && !inputTransition->second->GetConditional())
            {
                conditionally = true;
            } 
            else
            {
                conditionally  = false;
            }
            map<string, time_leak::Place*> outputPlaces = inputTransition->second->GetOutElements();
            for (auto outputPlace = outputPlaces.begin(); outputPlace != outputPlaces.end(); ++outputPlace)
            {
                if (outputPlace->second->GetId() != inputPlace->second->GetId())
                {
                    map<string, time_leak::Transition*> outputTransitions = outputPlace->second->GetOutElements();
                    for (auto outputTransition = outputTransitions.begin(); outputTransition != outputTransitions.end(); ++outputTransition)
                    {
                        if ((outputTransition->second->CheckIfLow() || outputTransition->second->GetTransitionType() == Transition::TransitionType::lowStart) && !outputTransition->second->GetConditional() && inputPlace->second->GetHighOut() < 2)
                        {
                            conditionally = true;
                            break;
                        }
                    }
                }
                if (conditionally)
                    break;
            }
            if (conditionally)
                break;
        }
        if (!conditionally)
        {
            conditionallyLowStart = false;
        }
    }

    if (conditionallyLowStart)
    {
        if (transition->GetTransitionType() == Transition::TransitionType::lowEnd)
            transition->SetTransitionType(Transition::TransitionType::low);
        else
            transition->SetTransitionType(Transition::TransitionType::lowStart);

        transition->SetConditional();
    }

    return;
}

void time_leak::NetAnalyzer::checkConditionallyLowEnd(time_leak::Transition *transition)
{
    map<string, Place*> outputPlaces = transition->GetOutElements();
    bool conditionallyLowEnd = false;

    for (auto outputPlace = outputPlaces.begin(); outputPlace != outputPlaces.end(); ++outputPlace)
    {
        map<string, Transition*> outputTransitions = outputPlace->second->GetOutElements();
        for (auto outputTransition = outputTransitions.begin(); outputTransition != outputTransitions.end(); ++outputTransition)
        {
            if ((outputTransition->second->CheckIfLow() || outputTransition->second->GetTransitionType() == Transition::TransitionType::lowStart) && !outputTransition->second->GetConditional())
            {
                conditionallyLowEnd = true;
                break;
            }
        }
        if (conditionallyLowEnd)
            break;
    }

    if (conditionallyLowEnd)
    {
        if (transition->GetTransitionType() == Transition::TransitionType::lowStart)
        {
            transition->SetTransitionType(Transition::TransitionType::low);
            checkIntervalOnlyCase(transition);
        }
        else
            transition->SetTransitionType(Transition::TransitionType::lowEnd);

        transition->SetConditional();
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