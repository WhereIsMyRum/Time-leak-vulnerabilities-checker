#include <iostream>
#include <cstdio>
#include <map>
#include <iterator>
#include <queue>

#include "include/time_leak/place.hpp"
#include "include/time_leak/transition.hpp"
#include "include/time_leak/element.hpp"
#include "include/time_leak/elementUniqueFifo.hpp"
#include "include/time_leak/net.hpp"
#include "include/time_leak/netParser.hpp"
#include "include/time_leak/netAnalyzer.hpp"
#include "include/time_leak/netPruner.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    
    time_leak::NetParser::CheckArguments(argc);

    bool runConditional = false;
    if (argc == 3 && *argv[2] == '1') 
        runConditional = true;

    time_leak::Net *n = new time_leak::Net(time_leak::NetParser::ParseNet(argv[1]));

    time_leak::NetPruner nPruner;
    nPruner.PruneNet(n);

    //n->PrintNet();

    time_leak::NetAnalyzer nAnalyzer;
    nAnalyzer.RunAnalysis(*n, runConditional);

}