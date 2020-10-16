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

using namespace std;

int main(int argc, char *argv[])
{

    time_leak::NetParser::CheckArguments(argc);
    time_leak::Net *n = new time_leak::Net(time_leak::NetParser::ParseNet(argv[1]));

    time_leak::NetAnalyzer nAnalyzer;
    nAnalyzer.RunAnalysis(*n);

}