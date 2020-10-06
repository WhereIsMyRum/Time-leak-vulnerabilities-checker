#include <string>
#include <iostream>
#include <map>
#include <iterator>

#include "../../include/time_leak/place.hpp"
#include "../../include/time_leak/transition.hpp"

using namespace std;
using namespace enums;


time_leak::Place::Place(string id)
    : Element(id)
{
}

bool time_leak::Place::IsTimeDeducible()
{
    return this->timeDeducible;
}


void time_leak::Place::AnalyzePlace()
{
    cout << "Analyzing " << this->id << endl;
    if (this->canTimeBeDeducted()) this->timeDeducible = true;

    analyzeIngoing();
    this->SetAnalyzed(true);

    if (this->canTimeBeDeducted()) this->timeDeducible = true;


}


bool time_leak::Place::canTimeBeDeducted()
{
    if (checkOutgoing() || checkIngoing())
    {
        return true;
    }
    return false;
}

bool time_leak::Place::checkOutgoing()
{
    map<string, Transition *>::iterator iterator;
    bool canBeDeduced = false;

    for (iterator = this->outElements.begin(); iterator != this->outElements.end(); ++iterator)
    {
        if (!iterator->second->IsHigh() || iterator->second->GetTransitionType() == TransitionType::lowStart)
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
    bool canBeDeduced = false;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->IsHigh() || iterator->second->GetTransitionType() == TransitionType::lowEnd)
        {
            canBeDeduced = true;
            break;
        }
    }
    return canBeDeduced;
}

void time_leak::Place::analyzeIngoing()
{
    map<string, Transition*>::iterator iterator;

    for (iterator = this->inElements.begin(); iterator != this->inElements.end(); ++iterator)
    {
        if (!iterator->second->WasAnalyzed()) iterator->second->AnalyzeTransition();
    }
}

