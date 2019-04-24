// Matt Whitesides
// CS6402 - Adv Data Mining
// 4/15/2019

// Facebook: Process took 26 minutes, 22 seconds and 365 milliseconds.

#include "../Include/preprocess.hpp"
#include "../Include/process.hpp"

using namespace NGraph;
using namespace std;

/**
	Traverses the given graph in a breath first fashion.

	@param graph: the graph to be traversed.
*/
void TraverseGraph(Graph g) {
	// for each node in the graph...
	for (Graph::const_iterator p = g.begin(); p != g.end(); p++) {
		cout << "Node: " << Graph::node(p) << "\n";

		// identify its neighbors
		Graph::vertex_set si = Graph::in_neighbors(p);
		Graph::vertex_set so = Graph::out_neighbors(p);

		Graph::vertex_set sio = si + so;

		// print out each out-going edge
		for (Graph::vertex_set::const_iterator t = sio.begin(); t != sio.end(); t++) {
			cout << *t << "\n";
		}
		cout << "\n";
	}
}

/**
	Apriori-based approach for finding frequent subgraphs.

	@param g: the graph to be mined.
	@param s: the minimum support threshold.
	@return a vector list of the frequent subgraphs of cardinality 1 to k.
*/
vector<Graph> AprioriBased(vector<Graph> ds, int s, int t = 1) {
	int k = 2;
	vector<Graph> subGraphs;
	vector<vector<Graph>> f;

	f.emplace_back(vector<Graph>());
	f.emplace_back(FrequentOneSubgraphs(ds, s));

	while (f[k - 1].size() != 0) {
		f.emplace_back(vector<Graph>());
		vector<Graph> c = CandidateGen(f[k - 1]);
		for (auto& g : c) {
			for (auto& gi : ds) {
				if (SubGraphIsomorphism(g, gi)) {
					++g.count;
					continue;
				}
			}

			if (g.count >= s && !GraphInSubGraphSet(subGraphs, g)) {
				subGraphs.emplace_back(g);
				f[k].emplace_back(g);
			}
		}
		k++;
	}

	return subGraphs;
}

/**
	Generates a list of candidates for a frequent subgraph using a modified level-wise join.

	@param g: The graph being considered.
	@param s: The minimum support threshold.
	@param f: The single subgraph to start off of.
	@return: A vector list of the frequent subgraphs.
*/
vector<Graph> CandidateGen(vector<Graph> ds) {
	vector<Graph> candidates;

	for (Graph& g : ds) {
		candidates.emplace_back(g);
		for (Graph::const_iterator p = g.begin(); p != g.end(); p++) {
			for (Graph& g2 : ds) {
				for (Graph::const_iterator p2 = g2.begin(); p2 != g2.end(); p2++) {
					auto node1 = Graph::node(p);
					auto node2 = Graph::node(p2);
					if (node1 != node2) {
						auto c = g;
						c.insert_undirected_edge(node1, node2);
						candidates.emplace_back(c);
					}
				}
			}
		}
	}

	//for (auto& x : candidates) {
	//	cout << "Candidate: " << endl << x << endl;
	//}

	return candidates;
}

/**
	Finds single frequent subgraphs in graphs ds.

	@param ds: The graph dataset.
	@param s: The minimum support threshold.
	@return: A vector list of the frequent subgraphs.
*/
vector<Graph> FrequentOneSubgraphs(vector<Graph> ds, int s) {
	// <Vertex, Count>
	map<int, int> vertexLookupTable;

	for (Graph& g : ds) {
		// for each node in the graph...
		for (Graph::const_iterator p = g.begin(); p != g.end(); p++) {
			if (vertexLookupTable.count(Graph::node(p)) > 0) {
				vertexLookupTable[Graph::node(p)] = ++vertexLookupTable[Graph::node(p)];
			}
			else {
				vertexLookupTable[Graph::node(p)] = 1;
			}
		}
	}

	vector<Graph> subGraphs;

	// for each value in the map
	for (auto& x : vertexLookupTable) {
		if (x.second >= s) {
			Graph gk;
			gk.insert_vertex(x.first);
			subGraphs.emplace_back(gk);
		}
	}

	return subGraphs;
}

/**
	Compares two graphs for isomorphism.

	@param x: The first graph being considered.
	@param y: The second graph being considered.
	@return: True if the two graphs are isomorphic.
*/
bool GraphIsomorphism(Graph x, Graph y) {
	if (x.num_edges() != y.num_edges()) return false;
	if (x.num_vertices() != y.num_vertices()) return false;

	return SubGraphIsomorphism(x, y);
}

/**
	Compares one subgraph to a full graph to check if it is contained.

	@param x: The subgraph graph being considered.
	@param y: The full graph being considered.
	@return: True if the two graphs are isomorphic.
*/
bool SubGraphIsomorphism(Graph sub, Graph g) {

	//cout << "Comparing:\n" << sub;
	//cout << "And:\n" << g;

	Graph::vertex_set subSet;

	for (Graph::const_iterator i = sub.begin(); i != sub.end(); i++) {
		subSet.insert(Graph::node(i));
	}

	auto isSub = g.subgraph(subSet);

	if (sub.num_vertices() != isSub.num_vertices() || sub.num_edges() != isSub.num_edges()) {
		return false;
	}

	Graph::const_iterator j = isSub.begin();
	for (Graph::const_iterator i = sub.begin(); i != sub.end(); i++) {
		auto si = Graph::in_neighbors(i) + Graph::out_neighbors(i);
		auto sj = Graph::in_neighbors(j) + Graph::out_neighbors(j);

		if (si.size() != sj.size()) return false;

		auto kj = sj.begin();
		for (auto& ki : si) {
			if (ki != *kj) return false;
			kj++;
		}

		j++;
	}

	//cout << "Isomorphic is true.\n";
	return true;
}

/**
	Check to see if a graph already exists in a dataset.

	@param ds: The graph dataset being considered.
	@param g: The graph being considered.
	@return: True if the graph is in the set.
*/
bool GraphInSubGraphSet(vector<Graph> ds, Graph g) {
	for (auto& x : ds) {
		if (GraphIsomorphism(x, g)) {
			return true;
		}
	}

	return false;
}

/**
	Converts a graph to an ordered list of nodes, by incoming and outgoing edge size.

	@param g: The graph being considered.
	@return: The list of nodes.
*/
vector<pair<Graph::vertex, int>> GraphToSortedNodeList(Graph g) {
	vector<pair<Graph::vertex, int>> ng;
	for (Graph::const_iterator p = g.begin(); p != g.end(); p++) {
		int si = Graph::in_neighbors(p).size();
		int so = Graph::out_neighbors(p).size();

		ng.emplace_back(pair<Graph::vertex, int>(Graph::node(p), si + so));
	}

	sort(ng.begin(), ng.end(), less_second<Graph::vertex, int>());
	return ng;
}
