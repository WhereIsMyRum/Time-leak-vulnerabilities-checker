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
    auto creationDuration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start); 

    time_leak::NetAnalyzer nAnalyzer1;
    start = std::chrono::high_resolution_clock::now();
    nAnalyzer1.RunAnalysis(*n, false);
    stop = std::chrono::high_resolution_clock::now();
    auto analysisNoPruningNoConditionalDuration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    n = new time_leak::Net(time_leak::NetParser::ParseNet(argv[1]));

    time_leak::NetPruner nPruner;
    start = std::chrono::high_resolution_clock::now();
    nPruner.PruneNet(n);

    time_leak::NetAnalyzer nAnalyzer2;
    nAnalyzer2.RunAnalysis(*n, false);
    stop = std::chrono::high_resolution_clock::now();
    auto analysisPruningNoConditional = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    n = new time_leak::Net(time_leak::NetParser::ParseNet(argv[1]));

    time_leak::NetAnalyzer nAnalyzer3;
    start = std::chrono::high_resolution_clock::now();
    nAnalyzer3.RunAnalysis(*n, true);
    stop = std::chrono::high_resolution_clock::now();
    auto analysisNoPruningConditional = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    /*n = new time_leak::Net(time_leak::NetParser::ParseNet(argv[1]));

    nPruner.PruneNet(n);
    time_leak::NetAnalyzer nAnalyzer4;
    start = std::chrono::high_resolution_clock::now();
    nAnalyzer4.RunAnalysis(*n, true);
    stop = std::chrono::high_resolution_clock::now();
    auto analysisPruningConditional = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);*/

    //cout << creationDuration.count() << " " << pruningDuration.count() << " " << analysisNoPruningNoConditionalDuration.count() << " " << annalysisPruningNoConditional.count() << " " << analysisNoPruningConditional.count() << " " << analysisPruningConditional.count() << endl;
    cout << creationDuration.count() << " " << analysisNoPruningNoConditionalDuration.count() << " " << analysisPruningNoConditional.count() << " " << analysisNoPruningConditional.count() << endl;
}