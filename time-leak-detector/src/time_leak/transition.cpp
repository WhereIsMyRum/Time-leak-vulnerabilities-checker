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
    }

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
    bool canBeDeduced = true;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->IsTimeDeducible())
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