#include "../../include/time_leak/netParser.hpp"

using namespace std;

void time_leak::NetParser::CheckArguments(int argc)
{
    if (argc == 0)
    {
        cout << "No arguments passed, exiting." << endl;
        exit(1);
    }
}

rapidjson::Document time_leak::NetParser::ParseNet(char *netSource)
{
    if (strstr(netSource, FILE_EXT) != NULL)
    {
        return ReadFile(netSource);
    }
    return ReadDirectlyFromCommandLine(netSource);
}

rapidjson::Document time_leak::NetParser::ReadFile(char *fileName)
{
    char readBuffer[65536];
    rapidjson::Document d;
    FILE *fp = fopen(fileName, "rb");

    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    d.ParseStream(is);

    fclose(fp);

    return d;
}

rapidjson::Document time_leak::NetParser::ReadDirectlyFromCommandLine(char *netString)
{
    rapidjson::Document d;
    d.Parse(netString);

    return d;
}