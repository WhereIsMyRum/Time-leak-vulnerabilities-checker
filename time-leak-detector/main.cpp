#include <iostream>
#include <cstdio>
#include <map>
#include <iterator>

#include "include/time_leak/globals.h"
#include "include/time_leak/place.h"
#include "include/time_leak/transition.h"
#include "include/time_leak/element.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

using namespace std;

bool ArgumentsPassed(int argc);
void ParseNet(char *netSource, rapidjson::Document &d);
void PopulatePlacesAndTransitions(rapidjson::Document &net);

namespace globals {
    std::map<std::string, time_leak::Place*> Places;
    std::map<std::string, time_leak::Transition*> Transitions;
}

int main(int argc, char *argv[])
{
    if (!ArgumentsPassed(argc)) exit(1);
    rapidjson::Document net;
    ParseNet(argv[1], net);

    PopulatePlacesAndTransitions(net);

    map<string, time_leak::Place*>::iterator it;

    for (it = globals::Places.begin(); it != globals::Places.end(); ++it) {
        cout << it->first << " : " << it->second->GetId() << endl;
    }

    map<string, time_leak::Transition*>::iterator it2;

    for (it2 = globals::Transitions.begin(); it2 != globals::Transitions.end(); ++it2) {
        cout << it2->first << " : " << it2->second->GetId() << endl;
    }
}



void ParseNet(char *netSource, rapidjson::Document &d) {
    void ReadDirectlyFromCommandLine();
    rapidjson::Document ReadFile(char *fileName);

     if (strstr(netSource, FILE_EXT) != NULL) {
        d = ReadFile(netSource);
    } else {
        d = ReadFile(netSource);
    }
}

rapidjson::Document ReadFile(char *fileName) {
    char readBuffer[65536];
    rapidjson::Document d;
    FILE *fp = fopen(fileName, "rb");
    
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    d.ParseStream(is);

    fclose(fp);

    return d;
}

void ReadDirectlyFromCommandLine() {
    cout << "parsing from command line" << endl;
}

bool ArgumentsPassed(int argc) {
    if (argc == 0) {
        cout << "No arguments passed, exiting." << endl;
        return false;
    }
    return true;
}

void PopulatePlacesAndTransitions(rapidjson::Document &net)
{   rapidjson::Value::ConstValueIterator iterator;

    for (iterator = net["places"].Begin(); iterator != net["places"].End(); ++iterator) {
        if (globals::Places)
        globals::Places.insert(pair<string, time_leak::Place*>(iterator->GetString(), new time_leak::Place(iterator->GetString())));
    }

    for (iterator = net["transitions"]["high"].Begin(); iterator != net["transitions"]["high"].End(); ++iterator) {
        globals::Transitions.insert(pair<string, time_leak::Transition*>(iterator->GetString(), new time_leak::Transition(iterator->GetString())));
    }
}

