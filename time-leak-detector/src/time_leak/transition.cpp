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


void time_leak::Transition::AnalyzeDeeper()
{
    cout << "Analyzing " << this->id << endl;
    analyzeIngoing();
    this->SetAnalyzed(true);

    if (this->transitionType != TransitionType::low && this->transitionType == TransitionType::lowEnd)
    {
        this->transitionType = this->canDeduceStartTime() ? TransitionType::low : TransitionType::lowEnd;
    }

    if (this->transitionType != TransitionType::low && this->transitionType == TransitionType::lowStart)
    {
        this->transitionType = this->canDeduceEndTime() ? TransitionType::low : TransitionType::lowStart;
    }

    if (this->transitionType == TransitionType::high)
    {
        if (this->canDeduceEndTime()) this->transitionType = TransitionType::lowEnd;
        if (this->canDeduceStartTime()) this->transitionType = this->transitionType == TransitionType::lowEnd ? TransitionType::low : TransitionType::lowStart;
    }
}

void time_leak::Transition::AnalyzeFirstLevel()
{
    if (this->canDeduceEndTime()) this->transitionType = TransitionType::lowEnd;
    if (this->canDeduceStartTime()) this->transitionType = this->transitionType == TransitionType::lowEnd ? TransitionType::low : TransitionType::lowStart;
}


bool time_leak::Transition::canDeduceEndTime()
{
    if (!this->IsHigh()) return true;

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
    if (!this->IsHigh()) return true;

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

void time_leak::Transition::analyzeIngoing()
{
    map<string, Place*>::iterator iterator;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->WasAnalyzed()) 
        {
            iterator->second->AnalyzeFirstLevel();
            globals::PlacesAnalyzeQueue.Push(iterator->second);
        }
    }

    if (globals::PlacesAnalyzeQueue.Size() > 0)
    {
        Place *place = globals::PlacesAnalyzeQueue.Pop();
        place->AnalyzeDeeper();
    }
}


