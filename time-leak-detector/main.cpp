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
#include "include/time_leak/net.hpp"
#include "include/rapidjson/document.h"
#include "include/rapidjson/filereadstream.h"

using namespace std;

bool CheckForArgumentsPassed(int argc);
void ParseNet(char *netSource, rapidjson::Document &d);
void PopulateTransitionTypeToString();

namespace globals
{
    map<enums::TransitionType, string> TransitionTypeToString;
} // namespace globals

int main(int argc, char *argv[])
{
    if (!CheckForArgumentsPassed(argc))
        exit(1);

    rapidjson::Document net;
    ParseNet(argv[1], net);
    PopulateTransitionTypeToString();

    time_leak::Net *n = new time_leak::Net(net);

    n->RunAnalysis();
    n->PrintResults();
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
    rapidjson::Document ReadDirectlyFromCommandLine(char *netString);

    if (strstr(netSource, FILE_EXT) != NULL)
    {
        d = ReadFile(netSource);
    }
    else
    {
        d = ReadDirectlyFromCommandLine(netSource);
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

rapidjson::Document ReadDirectlyFromCommandLine(char *netString)
{
    rapidjson::Document d;
    d.Parse(netString);

    return d;
}

void PopulateTransitionTypeToString()
{
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::high, "high"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::low, "low"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::lowEnd, "lowEnd"));
    globals::TransitionTypeToString.insert(pair<enums::TransitionType, string>(enums::TransitionType::lowStart, "lowStart"));
}
