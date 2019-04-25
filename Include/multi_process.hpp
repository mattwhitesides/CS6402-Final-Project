// Matt Whitesides
// CS6402 - Adv Data Mining
// 4/17/2019

#include <chrono> 
#include <fstream>
#include <future>
#include <stdio.h>
#include <string>
#include <thread>
#include <vector>

#include "ngraph.hpp"

using namespace NGraph;
using namespace std;
using namespace std::chrono;

void TraverseGraph(Graph);

vector<Graph> AprioriBased(vector<Graph>, int, int);

vector<Graph> FrequentOneSubgraphs(vector<Graph>, int);

vector<Graph> CandidateGen(vector<Graph>, int);

void AddCandidates(Graph, vector<Graph>, promise<vector<Graph>>&&);

vector<vector<Graph>> SplitVectorIntoSubVectors(vector<Graph>, int);

void AddCandidateIfValid(vector<Graph>, vector<Graph>, int, promise<vector<Graph>>&&);

bool GraphIsomorphism(Graph, Graph);

bool SubGraphIsomorphism(Graph, Graph);

bool GraphInSubGraphSet(vector<Graph>, Graph);

vector<pair<Graph::vertex, int>> GraphToSortedNodeList(Graph);

template <typename T1, typename T2>
struct less_second {
	typedef pair<T1, T2> type;
	bool operator ()(type const& a, type const& b) const {
		return a.second < b.second;
	}
};