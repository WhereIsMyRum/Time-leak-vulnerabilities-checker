#include "../../include/time_leak/netPruner.hpp"

using namespace std;

void time_leak::NetPruner::PruneNet(time_leak::Net *net)
{
    map<string, Transition*> lowTransitions = net->GetLowTransitions();

    for (auto iterator = lowTransitions.begin(); iterator != lowTransitions.end(); ++iterator)
    {
        switch (this->getPruningCase(iterator->second))
        {
            case pruningCase::oneToOne:
                cout << iterator->second->GetId() << " oneToOne" << endl;
                pruneOneToOne(iterator->second, net);
            break;
            case pruningCase::oneToManySingle:
                cout << iterator->second->GetId() << " oneToManySingle" << endl;
                pruneOneToManySingle(iterator->second, net);
            break;
            case pruningCase::manyToOneSingle:
                cout << iterator->second->GetId() << " manyToOneSingle" << endl;
                pruneManyToOneSingle(iterator->second, net);
            break;
            default:
                continue;
            break;
        }
    }
}

time_leak::NetPruner::pruningCase time_leak::NetPruner::getPruningCase(Transition *transition)
{
    pruningCase pCase;
    if (transition->GetInElements().size() == 1)
    {
        if (transition->GetOutElements().size() == 1)
        {
            pCase = pruningCase::oneToOne;
        }
        else
        {
            pCase = pruningCase::oneToManySingle;
        }
    }
    else if (transition->GetOutElements().size() == 1)
    {
        pCase = pruningCase::manyToOneSingle;
    }
    else
    {
        pCase = pruningCase::notPrunable;
    }

    pCase = !this->checkIfAllLow(transition) ? pruningCase::notPrunable : pCase;

    if (pCase == pruningCase::manyToOneSingle || pCase == pruningCase::oneToManySingle)
    {
        pCase = !this->checkIfSingle(transition) ? pruningCase::notPrunable : pCase;
    }

    return pCase;
}

bool time_leak::NetPruner::checkIfAllLow(Transition *transition)
{
    bool allLow = true;
    map<string, time_leak::Place *> places = transition->GetInElements();
    for (auto iterator = places.begin(); iterator != places.end(); ++iterator)
    {
        if (iterator->second->GetInElements().size() == 0)
        {
            return false;
        }

        map<string, time_leak::Transition *> transitions = iterator->second->GetInElements();

        for (auto iteratorInner = transitions.begin(); iteratorInner != transitions.end(); ++iteratorInner)
        {
            if (iteratorInner->second->IsHigh())
            {
                allLow = false;
                break;
            }
        }
    }

    places = transition->GetOutElements();
    for (auto iterator = places.begin(); iterator != places.end(); ++iterator)
    {
        if (iterator->second->GetOutElements().size() == 0)
        {
            return false;
        }

        map<string, time_leak::Transition *> transitions = iterator->second->GetOutElements();

        for (auto iteratorInner = transitions.begin(); iteratorInner != transitions.end(); ++iteratorInner)
        {
            if (iteratorInner->second->IsHigh() && allLow)
            {
                allLow = false;
                break;
            }
        }
    }

    return allLow;
}

bool time_leak::NetPruner::checkIfSingle(Transition *transition)
{
    if (transition->GetOutElements().size() == 1)
    {
        Place *place = transition->GetOutElements().begin()->second;
        if (place->GetOutElements().size() > 1)
        {
            return false;
        }
    }
    else
    {
        Place *place = transition->GetInElements().begin()->second;
        if (place->GetInElements().size() > 1)
        {
            return false;
        }
    }

    return true;

}

void time_leak::NetPruner::pruneOneToOne(Transition *transition, Net *net)
{
    Place *startPlace = transition->GetInElements().begin()->second;
    Place *removePlace = transition->GetOutElements().begin()->second;

    startPlace->RemoveOutElement(transition->GetId());

    map<string, Transition *> removePlaceOutTransitions = removePlace->GetOutElements();
    for (auto iterator = removePlaceOutTransitions.begin(); iterator != removePlaceOutTransitions.end(); ++iterator)
    {
        iterator->second->RemoveInElement(removePlace->GetId());
        iterator->second->AddInElement(startPlace);
        startPlace->AddOutElement(iterator->second);
    }

    net->RemovePlace(removePlace->GetId());
    net->RemoveTransition(transition->GetId());
}

void time_leak::NetPruner::pruneOneToManySingle(Transition *transition, Net *net)
{
    map<string, Place *> outputPlaces = transition->GetOutElements();
    Place *inputPlace = transition->GetInElements().begin()->second;
    Transition *inputTransition = inputPlace->GetInElements().begin()->second;

    inputTransition->RemoveOutElement(inputPlace->GetId());

    for (auto iterator = outputPlaces.begin(); iterator != outputPlaces.end(); ++iterator)
    {
        iterator->second->RemoveInElement(transition->GetId());
        iterator->second->AddInElement(inputTransition);

        inputTransition->AddOutElement(iterator->second);
    }

    net->RemovePlace(inputPlace->GetId());
    net->RemoveTransition(transition->GetId());
}

void time_leak::NetPruner::pruneManyToOneSingle(Transition *transition, Net *net)
{
    map<string, Place *> inputPlaces = transition->GetInElements();
    Place *outputPlace = transition->GetOutElements().begin()->second;
    Transition *outputTransition = outputPlace->GetOutElements().begin()->second;

    outputTransition->RemoveInElement(outputPlace->GetId());

    for (auto iterator = inputPlaces.begin(); iterator != inputPlaces.end(); ++iterator)
    {
        iterator->second->RemoveOutElement(transition->GetId());
        iterator->second->AddOutElement(outputTransition);

        outputTransition->AddInElement(iterator->second);
    }

    net->RemovePlace(outputPlace->GetId());
    net->RemoveTransition(transition->GetId());
}