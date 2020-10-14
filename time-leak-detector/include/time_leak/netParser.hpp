#ifndef NETPARSER
#define NETPARSER

#define FILE_EXT ".json"

#include <iostream>

#include "../rapidjson/document.h"
#include "../rapidjson/filereadstream.h"

namespace time_leak 
{
    class NetParser
    {
        public:
            static void CheckArguments(int argc);
            static rapidjson::Document ParseNet(char *netSource);
            static rapidjson::Document ReadFile(char *fileName);
            static rapidjson::Document ReadDirectlyFromCommandLine(char *netString);
    };
}

#endif