// Matt Whitesides
// CS6402 - Adv Data Mining
// 4/15/2019

#include "..\Include\preprocess.hpp"
#include "..\Include\multi_process.hpp"

/**
	Traverses the given graph in a breath first fashion.

	@param graph: The graph to be traversed.
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

	@param g: The graph to be mined.
	@param s: The minimum support threshold.
	@param t: The minimum support threshold.
	@return: A vector list of the frequent subgraphs of cardinality 1 to k.
*/
vector<Graph> AprioriBased(vector<Graph> ds, int s, int t = 1) {
	unsigned int k = 2;
	vector<Graph> subGraphs;
	vector<vector<Graph>> f;

	f.emplace_back(vector<Graph>());
	f.emplace_back(FrequentOneSubgraphs(ds, s));

	while (f[k - 1].size() != 0) {
		auto start = high_resolution_clock::now();

		f.emplace_back(vector<Graph>());

		vector<Graph> c = CandidateGen(f[k - 1], t);

		vector<thread> threads;
		vector<future<vector<Graph>>> futures;
		auto subCandidates = SplitVectorIntoSubVectors(c, t);

		int ti = 1;
		for (auto& sc : subCandidates) {
			promise<vector<Graph>> p;
			futures.emplace_back(p.get_future());
			threads.emplace_back(thread(AddCandidateIfValid, sc, ds, s, ti, move(p)));
			++ti;
		}

		for (auto& t : threads) {
			t.join();
		}

		for (auto& future : futures) {
			for (auto& g : future.get()) {
				if (!GraphInSubGraphSet(subGraphs, g)) {
					subGraphs.emplace_back(g);
					f[k].emplace_back(g);
				}
			}
		}
		
		k++;

		threads.clear();
		futures.clear();
		c.clear();

		// Stop the timer and get the duration.
		auto stop = high_resolution_clock::now();
		auto durationMil = duration_cast<milliseconds>(stop - start);
		auto durationSec = duration_cast<seconds>(stop - start);
		auto durationMin = duration_cast<minutes>(stop - start);

		cout << "Analyzed " << c.size() << " candidates with K-level " << k - 1 << " using " << t <<  " threads. Process took " << durationMin.count() << " minutes, " << (durationSec.count() % 60)
			<< " seconds and " << (durationMil.count() % 1000) << " milliseconds." << endl;
	}

	cout << endl;
	return subGraphs;
}

void AddCandidateIfValid(vector<Graph> c, vector<Graph> ds, int s, int t, promise<vector<Graph>> && p) {
	vector<Graph> subGraphs;

	for (auto& g : c) {
		for (auto& gi : ds) {
			if (SubGraphIsomorphism(g, gi)) {
				++g.count;
			}
		}

		if (g.count >= s && !GraphInSubGraphSet(subGraphs, g)) {
			subGraphs.emplace_back(g);
		}
	}

	p.set_value(subGraphs);
}

/**
	Generates a list of candidates for a frequent subgraph using a modified level-wise join.

	@param g: The graph being considered.
	@param s: The minimum support threshold.
	@param f: The single subgraph to start off of.
	@return: A vector list of the frequent subgraphs.
*/
vector<Graph> CandidateGen(vector<Graph> ds, int t = 1) {
	vector<Graph> candidates;
	auto subVectors = SplitVectorIntoSubVectors(ds, t);

	for (Graph& g : ds) {
		vector<thread> threads;
		vector<future<vector<Graph>>> futures;
		candidates.emplace_back(g);

		for (auto& sg : subVectors) {
			promise<vector<Graph>> p;
			futures.emplace_back(p.get_future());
			threads.emplace_back(thread(AddCandidates, g, sg, move(p)));
		}

		for (auto& t : threads) {
			t.join();
		}

		for (auto& f : futures) {
			for (auto& g : f.get()) {
				candidates.emplace_back(g);
			}
		}

		threads.clear();
		futures.clear();
	}

	return candidates;
}

vector<vector<Graph>> SplitVectorIntoSubVectors(vector<Graph> v, int n) {
	vector<vector<Graph>> result;
	int vSize = v.size();
	int size = vSize / n;
	int remain = vSize % n;

	if (vSize < n) {
		vector<Graph> temp;

		for (int j = 0; j < remain; ++j) {
			temp.emplace_back(v[j]);
		}

		result.emplace_back(temp);
		return result;
	}

	int total = 0;
	for (int i = 0; i < n; ++i) {
		vector<Graph> temp;
		for (int j = 0; j < size; ++j) {
			temp.emplace_back(v[total++]);
		}
		if (i == n - 1) {
			for (int j = 0; j < remain; ++j) {
				temp.emplace_back(v[total++]);
			}
		}
		result.emplace_back(temp);
	}

	return result;
}

void AddCandidates(Graph g, vector<Graph> ds, promise<vector<Graph>> && p) {
	vector<Graph> candidates;

	for (Graph& g2 : ds) {
		for (Graph::const_iterator p = g.begin(); p != g.end(); p++) {
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

	p.set_value(candidates);
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