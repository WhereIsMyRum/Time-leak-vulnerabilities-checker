#include "../../include/time_leak/transition.hpp"

using namespace std;

time_leak::Transition::Transition(string id, bool high, TransitionType type)
    : Element(id)
{
    this->transitionType = type;
    this->highT = high;
}

time_leak::Transition::TransitionType time_leak::Transition::GetTransitionType()
{
    return this->transitionType;
}

void time_leak::Transition::SetTransitionType(time_leak::Transition::TransitionType t)
{
    this->transitionType = t;
}

void time_leak::Transition::SetConditional(bool value)
{
    this->conditional = value;
}

bool time_leak::Transition::GetConditional()
{
    return this->conditional;
}

string time_leak::Transition::GetTransitionTypeString()
{
    string transitionString;
    switch (this->transitionType)
    {
        case TransitionType::high:
            transitionString = "high";
            break;
        case  TransitionType::low:
            transitionString ="low";
            break;
        case TransitionType::lowEnd:
            transitionString = "lowEnd";
            break;
        case TransitionType::lowStart:
            transitionString = "lowStart";
            break;
        case TransitionType::maxDuration:
            transitionString = "maxDuration";
            break;
    }

    if (this->conditional)
        transitionString.append("-conditional");

    return transitionString;
}

bool time_leak::Transition::IsHigh()
{
    return this->highT;
}

bool time_leak::Transition::CheckIfLow()
{
    return !this->highT || this->transitionType == Transition::TransitionType::low;
}

bool time_leak::Transition::canDeduceEndTime()
{
    if (!this->IsHigh())
        return true;

    map<string, Place *>::iterator iterator;
    bool canBeDeduced = false;

    for (iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (iterator->second->GetEndTimeDeducible())
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
    bool canBeDeduced = true;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->GetStartTimeDeducible())
        {
            canBeDeduced = false;
            break;
        }
    }
    return canBeDeduced;
}

bool time_leak::Transition::Analyze()
{
    //cout << "Analyzing " << this->id << endl;
    TransitionType initialVal = this->transitionType;

    if (this->canDeduceEndTime())
        this->transitionType = TransitionType::lowEnd;
    if (this->canDeduceStartTime())
        this->transitionType = this->transitionType == TransitionType::lowEnd ? TransitionType::low : TransitionType::lowStart;

    this->SetAnalyzed(true);

    if (initialVal != this->transitionType)
        return true;
    return false;
}