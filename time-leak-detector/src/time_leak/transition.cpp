#include "../../include/time_leak/transition.hpp"

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

bool time_leak::Transition::CheckIfLow()
{
    return !this->high || this->transitionType == TransitionType::low;
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