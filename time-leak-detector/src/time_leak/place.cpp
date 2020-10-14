#include "../../include/time_leak/place.hpp"

using namespace std;

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
    bool deducable = false;

    if (checkOutgoing())
        deducable = true;
    if (checkIngoing())
        deducable = true;

    return deducable;
}

bool time_leak::Place::checkOutgoing()
{
    map<string, Transition *>::iterator iterator;
    bool canBeDeduced = false;

    for (iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (iterator->second->CheckIfLow() || iterator->second->GetTransitionType() == Transition::TransitionType::lowStart)
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
        if (!iterator->second->CheckIfLow() && iterator->second->GetTransitionType() != Transition::TransitionType::lowEnd)
        {
            canBeDeduced = false;
            break;
        }
    }
    return canBeDeduced;
}

bool time_leak::Place::Analyze()
{
    //cout << "Analyzing " << this->id << endl;
    bool initialVal = this->timeDeducible;
    if (this->canTimeBeDeduced())
        this->timeDeducible = true;

    this->SetAnalyzed(true);

    if (initialVal != this->timeDeducible)
        return true;
    return false;
}