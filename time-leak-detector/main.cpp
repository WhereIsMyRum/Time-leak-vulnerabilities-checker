#include <iostream>
#include <cstdio>
#include <map>
#include <iterator>
#include <queue>

#include "include/time_leak/globals.hpp"
#include "include/time_leak/place.hpp"
#include "include/time_leak/transition.hpp"
#include "include/time_leak/element.hpp"
#include "include/time_leak/elementUniqueFifo.hpp"
#include "include/time_leak/enums.hpp"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

using namespace std;

bool CheckForArgumentsPassed(int argc);
void ParseNet(char *netSource, rapidjson::Document &d);
void PopulatePlacesAndTransitions(rapidjson::Document &net);
void CreateTransitionBackwardLink(string transitionId, time_leak::Place *place);
void CreateLinks(rapidjson::Document &net);
void PrintNet();
void RunAnalysis();
void PrintResults();
void PopulateTransitionTypeToString();
template <class T>
void ResetAnalyzedFlag(T &Elements);

namespace globals
{
    map<std::string, time_leak::Place *> Places;
    map<std::string, time_leak::Transition *> Transitions;
    map<enums::TransitionType, string> TransitionTypeToString;
    time_leak::ElementUniqueFifo<time_leak::Place *> PlacesQueue;
    time_leak::ElementUniqueFifo<time_leak::Transition *> TransitionsQueue;
} // namespace globals

int main(int argc, char *argv[])
{
    if (!CheckForArgumentsPassed(argc))
        exit(1);

    rapidjson::Document net;
    ParseNet(argv[1], net);
    PopulateTransitionTypeToString();

    PopulatePlacesAndTransitions(net);
    CreateLinks(net);

    PrintNet();
    RunAnalysis();
    PrintResults();
}

bool CheckForArgumentsPassed(int argc)
{
    if (argc == 0)
    {
        cout << "No arguments passed, exiting." << endl;
        return false;
    }
    return true;
}

void ParseNet(char *netSource, rapidjson::Document &d)
{
    void ReadDirectlyFromCommandLine();
    rapidjson::Document ReadFile(char *fileName);

    if (strstr(netSource, FILE_EXT) != NULL)
    {
        d = ReadFile(netSource);
    }
    else
    {
        d = ReadFile(netSource);
    }
}

rapidjson::Document ReadFile(char *fileName)
{
    char readBuffer[65536];
    rapidjson::Document d;
    FILE *fp = fopen(fileName, "rb");

    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    d.ParseStream(is);

    fclose(fp);

    return d;
}

void ReadDirectlyFromCommandLine()
{
    cout << "parsing from command line" << endl;
}

void PopulatePlacesAndTransitions(rapidjson::Document &net)
{
    void PopulateTransitions(rapidjson::Document & net);
    rapidjson::Value::ConstValueIterator iterator;

    time_leak::PopulatePlaces(net);
    time_leak::PopulateTransitions(net);
}

void PopulateTransitionTypeToString()
{
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::high, "High"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::low, "Low"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::lowEnd, "LowEnd"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::lowStart, "LowStart"));
}

void CreateLinks(rapidjson::Document &net)
{
    time_leak::CreatePlacesForwardLinks(net);
    time_leak::CreateTransitionsForwardLinks(net);
}

void PrintNet()
{
    cout << "Places: " << globals::Places.size() << endl;
    cout << "Transitions: " << globals::Transitions.size() << endl;
    globals::Places.at("end")->Print();
}

void RunAnalysis()
{
    void AnalyzeNet(time_leak::Place * startPlace, bool upwards);

    // analyze bottom-up
    AnalyzeNet(globals::Places.at("end"), true);

    globals::TransitionsQueue.Clear();
    globals::PlacesQueue.Clear();

    ResetAnalyzedFlag(globals::Places);
    ResetAnalyzedFlag(globals::Transitions);

    // analyze top-down
    AnalyzeNet(time_leak::FindStartPlace(), false);
}

void AnalyzeNet(time_leak::Place *startPlace, bool upwards)
{
    startPlace->Analyze();
    startPlace->Traverse(startPlace->GetElementsBasedOnDirection(upwards), globals::TransitionsQueue);

    while (globals::PlacesQueue.Size() > 0 || globals::TransitionsQueue.Size() > 0)
    {
        time_leak::ForwardQueue(globals::TransitionsQueue, globals::PlacesQueue, upwards);
        time_leak::ForwardQueue(globals::PlacesQueue, globals::TransitionsQueue, upwards);
    }
}

template <class T>
void ResetAnalyzedFlag(T &Elements)
{
    for (auto iterator = Elements.begin(); iterator != Elements.end(); ++iterator)
    {
        iterator->second->SetAnalyzed(false);
    }
}

void PrintResults()
{
    map<string, time_leak::Transition *>::iterator iterator;
    for (iterator = globals::Transitions.begin(); iterator != globals::Transitions.end(); ++iterator)
    {
        cout << iterator->second->GetId() << " is " << globals::TransitionTypeToString.at(iterator->second->GetTransitionType()) << endl;
    }

    map<string, time_leak::Place *>::iterator iterator2;
    for (iterator2 = globals::Places.begin(); iterator2 != globals::Places.end(); ++iterator2)
    {
        if (iterator2->second->IsTimeDeducible())
            cout << iterator2->second->GetId() << " is timeDeducible." << endl;
    }
}