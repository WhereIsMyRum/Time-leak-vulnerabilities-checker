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
    this->parallelIn = false;
    this->parallelOut = false;

    if (checkOutgoing())
        deducable = true;
    if (checkIngoing())
        deducable = true;
    if (deducable)
        deducable = checkParallelCase();

    return deducable;
}

bool time_leak::Place::checkOutgoing()
{
    map<string, Transition *>::iterator iterator;
    this->highOut = 0;
    bool canBeDeduced = this->outElements.size() > 0 ? true : false;

    for (iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (!iterator->second->CheckIfLow() && iterator->second->GetTransitionType() != Transition::TransitionType::lowStart)
        {
            canBeDeduced = false;
            ++this->highOut;
        }
    }

    return canBeDeduced;
}

bool time_leak::Place::checkIngoing()
{
    map<string, Transition *>::iterator iterator;
    this->highIn = 0;
    bool canBeDeduced = this->inElements.size() > 0 ? true : false;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->CheckIfLow() && iterator->second->GetTransitionType() != Transition::TransitionType::lowEnd)
        {
            canBeDeduced = false;
            ++this->highIn;
        }
    }
    return canBeDeduced;
}

bool time_leak::Place::checkParallelCase()
{
    if ((this->inElements.size() == 1 && this->outElements.size() < 3) || (this->inElements.size() < 3 && this->outElements.size() == 1))
        return true;

    if ((this->highIn > 1 && this->highOut > 0 && this->outElements.size() > 0))
    {
        this->parallelIn = true;
        return false;

    }

    if ((this->highOut > 1 && this->highIn >= 0 && this->inElements.size() > 0))
    {
        this->parallelOut = true;
        return false;
    }

    return true;
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