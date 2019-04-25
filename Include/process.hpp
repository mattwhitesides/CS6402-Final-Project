// Matt Whitesides
// CS6402 - Adv Data Mining
// 4/15/2019

#include <chrono> 
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

#include "ngraph.hpp"

using namespace NGraph;
using namespace std;
using namespace std::chrono;

void TraverseGraph(Graph);

vector<Graph> AprioriBased(vector<Graph>, int, int);

vector<Graph> FrequentOneSubgraphs(vector<Graph>, int);

vector<Graph> CandidateGen(vector<Graph>);

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