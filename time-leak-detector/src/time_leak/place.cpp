#include "../../include/time_leak/place.hpp"

using namespace std;

time_leak::Place::Place(string id)
    : Element(id)
{
}

bool time_leak::Place::GetEndTimeDeducible()
{
    return this->endTimeDeducible;
}

bool time_leak::Place::GetStartTimeDeducible()
{
    return this->startTimeDeducible;
}

bool time_leak::Place::isEndTimeDeducible()
{
    if (this->inElements.size() == 0 || this->outElements.size() == 0)
        return false;

    if (this->highOut == 0)
        return true;

    if (this->highOut != this->outElements.size())
    {
        this->conditionallyLowEnd = true;
        return true;
    }
    

    return false;
}

bool time_leak::Place::isStartTimeDeducible()
{
    if (this->inElements.size() == 0 || this->outElements.size() == 0)
        return false;

    if (this->highIn == 0)
    {
        if (this->highOut < 2)
            return true;
        return false;
    }

    if (this->highIn != this->inElements.size())
    {
        this->conditionallyLowStart = true;
        return true;
    }

    return false;
}

void time_leak::Place::canTimeBeDeduced()
{

    this->endTimeDeducible = isEndTimeDeducible();
    this->startTimeDeducible = isStartTimeDeducible();
}

void time_leak::Place::countHighInAndHighOut()
{
    this->highIn = 0;
    this->highOut = 0;

    for (auto iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (!iterator->second->CheckIfLow() && iterator->second->GetTransitionType() != Transition::TransitionType::lowStart || ((iterator->second->GetTransitionType() == Transition::TransitionType::lowStart || iterator->second->GetTransitionType() == Transition::TransitionType::low) && iterator->second->IsConditionallyLowStart() && this->GetId() == iterator->second->conditionallyLowStartId))
        {
            ++highOut;
        }
    }

    for (auto iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->CheckIfLow() && iterator->second->GetTransitionType() != Transition::TransitionType::lowEnd || ((iterator->second->GetTransitionType() == Transition::TransitionType::lowEnd || iterator->second->GetTransitionType() == Transition::TransitionType::low) && iterator->second->IsConditionallyLowEnd() && this->GetId() == iterator->second->conditionallyLowEndId))
        {
            highIn++;
        }
    }
}

bool time_leak::Place::Analyze()
{
    bool initialValEnd = this->endTimeDeducible;
    bool initalValStart = this->startTimeDeducible;

    this->conditionallyLowStart = false;
    this->conditionallyLowEnd = false;

    countHighInAndHighOut();
    canTimeBeDeduced();

    this->SetAnalyzed(true);

    if (initialValEnd != this->endTimeDeducible || initalValStart != this->startTimeDeducible)
        return true;

    return false;
}