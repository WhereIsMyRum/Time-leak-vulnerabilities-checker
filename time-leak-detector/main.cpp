#include <iostream>
#include <cstdio>
#include <map>
#include <iterator>
#include <queue>
#include <chrono>

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
    auto start = std::chrono::high_resolution_clock::now();
    time_leak::Net *n = new time_leak::Net(time_leak::NetParser::ParseNet(argv[1]));
    auto stop = std::chrono::high_resolution_clock::now();
    auto creationDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 

    time_leak::NetAnalyzer nAnalyzer;
    start = std::chrono::high_resolution_clock::now();
    nAnalyzer.RunAnalysis(*n);
    stop = std::chrono::high_resolution_clock::now();
    auto analysisNoPruningDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

    time_leak::NetPruner nPruner;
    start = std::chrono::high_resolution_clock::now();
    nPruner.PruneNet(n);
    stop = std::chrono::high_resolution_clock::now();
    auto pruningDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 

    start = std::chrono::high_resolution_clock::now();
    nAnalyzer.RunAnalysis(*n);
    stop = std::chrono::high_resolution_clock::now();
    auto analysisPruningDuration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start); 

    cout << "creation: " << creationDuration.count() << endl;
    cout << "analysis w/o pruning: " << analysisNoPruningDuration.count() << endl;
    cout << "pruning: " <<  pruningDuration.count() << endl;
    cout << "analysis w/pruning duration: " << analysisPruningDuration.count() << endl;
}