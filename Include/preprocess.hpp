// Matt Whitesides
// CS6402 - Adv Data Mining
// 3/19/2019

#include <stdio.h>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <vector>
#include "ngraph.hpp"

using namespace NGraph;

Graph CreateSampleGraph(int = 0);

vector<Graph> CreateGraphsFromDataSetDir(string, bool = true);

inline bool IsInteger(const string&);

void PrintGraphDetails(Graph);