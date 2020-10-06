#include <iostream>
#include <cstdio>
#include <map>
#include <iterator>
#include <queue>

#include "include/time_leak/globals.hpp"
#include "include/time_leak/place.hpp"
#include "include/time_leak/transition.hpp"
#include "include/time_leak/element.hpp"
#include "include/time_leak/enums.hpp"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

using namespace std;

bool ArgumentsPassed(int argc);
void ParseNet(char *netSource, rapidjson::Document &d);
void PopulatePlacesAndTransitions(rapidjson::Document &net);
void CreateTransitionBackwardLink(string transitionId, time_leak::Place* place);
void CreateLinks(rapidjson::Document &net);
void PrintNet();
void AnalyzeNet();
void SetupStartPlace();
void PrintResults();
void PopulateTransitionTypeToString();


namespace globals
{
    map<std::string, time_leak::Place *> Places;
    map<std::string, time_leak::Transition *> Transitions;
    map<enums::TransitionType, string> TransitionTypeToString;
    globals::UniqueFifo<time_leak::Place *> PlacesAnalyzeQueue;
    globals::UniqueFifo<time_leak::Transition *> TransitionsAnalyzeQueue;
} // namespace globals

int main(int argc, char *argv[])
{
    if (!ArgumentsPassed(argc))
        exit(1);

    rapidjson::Document net;
    ParseNet(argv[1], net);
    PopulateTransitionTypeToString();

    PopulatePlacesAndTransitions(net);
    CreateLinks(net);
    //SetupStartPlace();

    AnalyzeNet();
    PrintNet();
    PrintResults();
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

bool ArgumentsPassed(int argc)
{
    if (argc == 0)
    {
        cout << "No arguments passed, exiting." << endl;
        return false;
    }
    return true;
}


void PopulatePlacesAndTransitions(rapidjson::Document &net)
{
    void PopulatePlaces(rapidjson::Document &net);
    void PopulateTransitions(rapidjson::Document &net);

    rapidjson::Value::ConstValueIterator iterator;
    PopulatePlaces(net);
    PopulateTransitions(net);
}

void PopulatePlaces(rapidjson::Document &net)
{
    rapidjson::Value::ConstValueIterator iterator;
    for (iterator = net["places"].Begin(); iterator != net["places"].End(); ++iterator)
    {

        if (net["flows"]["places"].HasMember(iterator->GetString()))
        {
            globals::Places.insert(pair<string, time_leak::Place *>(iterator->GetString(), new time_leak::Place(iterator->GetString())));
        }
        else
        {
            globals::Places.insert(pair<string, time_leak::Place *>("end", new time_leak::Place(iterator->GetString())));
            globals::Places.at("end")->SetAnalyzed(true);
        }
    }

}

void PopulateTransitions(rapidjson::Document &net)
{
    rapidjson::Value::ConstValueIterator iterator;
    for (iterator = net["transitions"]["high"].Begin(); iterator != net["transitions"]["high"].End(); ++iterator)
    {
        globals::Transitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString())));
    }

    for (iterator = net["transitions"]["low"].Begin(); iterator != net["transitions"]["low"].End(); ++iterator)
    {
        globals::Transitions.insert(pair<string, time_leak::Transition *>(iterator->GetString(), new time_leak::Transition(iterator->GetString(), false, enums::TransitionType::low)));
    }

}

void SetupStartPlace()
{
    map<string, time_leak::Place*>::iterator iterator;

    for (iterator = globals::Places.begin(); iterator != globals::Places.end(); ++iterator)
    {
        if (iterator->second->GetInElementsSize() == 0)
        {
            iterator->second->SetAnalyzed(true);
        }
    }
}

void CreateLinks(rapidjson::Document &net)
{
    void CreatePlacesForwardLinks(rapidjson::Document &net);
    void CreateTransitionsForwardLinks(rapidjson::Document &net);

    CreatePlacesForwardLinks(net);
    CreateTransitionsForwardLinks(net);
}

void CreatePlacesForwardLinks(rapidjson::Document &net)
{
    void CreateTransitionBackwardLink(string transitionId, time_leak::Place* place);

    map<string, time_leak::Place *>::iterator iterator;
    for (iterator = globals::Places.begin(); iterator != globals::Places.end(); ++iterator)
    {
        const char *placeKey;
        placeKey = iterator->second->GetId().c_str();
        if (net["flows"]["places"].HasMember(placeKey)) {
            for (rapidjson::SizeType i = 0; i < net["flows"]["places"][placeKey].Size(); ++i)
            {
                string transitionId = net["flows"]["places"][placeKey][i].GetString();
                iterator->second->AddOutElement(globals::Transitions.at(transitionId));
                CreateTransitionBackwardLink(transitionId, iterator->second);
            }
        }
    }
}


void CreateTransitionsForwardLinks(rapidjson::Document &net)
{
    void CreatePlaceBackwardLink(string placeId, time_leak::Transition* transition);

    map<string, time_leak::Transition *>::iterator iterator;
    for (iterator = globals::Transitions.begin(); iterator != globals::Transitions.end(); ++iterator)
    {
        const char *transitionKey = iterator->second->GetId().c_str();
        for (rapidjson::SizeType i = 0; i < net["flows"]["transitions"][transitionKey].Size(); ++i)
        {
            string placeId = net["flows"]["transitions"][transitionKey][i].GetString();

            placeId = globals::Places.find(placeId) == globals::Places.end() ? "end" : placeId;

            iterator->second->AddOutElement(globals::Places.at(placeId));
            CreatePlaceBackwardLink(placeId, iterator->second);
        }
    }
}

void CreateTransitionBackwardLink(string transitionId, time_leak::Place* place)
{
    globals::Transitions.at(transitionId)->AddInElement(place);
}

void CreatePlaceBackwardLink(string transitionId, time_leak::Transition* transition)
{
    globals::Places.at(transitionId)->AddInElement(transition);
}

void PopulateTransitionTypeToString()
{
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::high, "High"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::low, "Low"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::lowEnd, "LowEnd"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::lowStart, "LowStart"));
}

void PrintNet()
{
    cout << "Places: " << globals::Places.size() << endl;
    cout << "Transitions: " << globals::Transitions.size() << endl;
    globals::Places.at("end")->Print();
}

void PrintResults()
{
    map<string, time_leak::Transition*>::iterator iterator;
    for (iterator = globals::Transitions.begin(); iterator != globals::Transitions.end(); ++iterator)
    {
        cout << iterator->second->GetId() << " is " << globals::TransitionTypeToString.at(iterator->second->GetTransitionType()) << endl;
    }

    map<string, time_leak::Place*>::iterator iterator2;
    for (iterator2 = globals::Places.begin(); iterator2 != globals::Places.end(); ++iterator2)
    {
        if (iterator2->second->IsTimeDeducible()) cout << iterator2->second->GetId() << " is timeDeducible." << endl;
    }

}

void AnalyzeNet()
{
    time_leak::Place *end = globals::Places.at("end");
    end->AnalyzeFirstLevel();
    end->AnalyzeDeeper();
}