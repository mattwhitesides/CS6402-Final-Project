// Matt Whitesides
// CS6402 - Adv Data Mining
// 3/19/2019

#include "../Include/preprocess.hpp"

using namespace NGraph;
using namespace std;

namespace fs = std::experimental::filesystem;

/**
	Create a test sample NGraph
	0: Default graph.
	1: Default graph w/ one edge changed.
	2: Isomorphic to the default graph.
	3: Default graph w/ one vert missing.

	@return: The created graph object.
*/
Graph CreateSampleGraph(int alt) {

	Graph sample;

	if (alt == 0) {
		sample.insert_undirected_edge(3, 4);
		sample.insert_undirected_edge(4, 8);
		sample.insert_undirected_edge(8, 6);
		sample.insert_undirected_edge(6, 9);
		sample.insert_undirected_edge(9, 5);
		sample.insert_undirected_edge(5, 1);
		sample.insert_undirected_edge(1, 3);
		sample.insert_undirected_edge(4, 2);
		sample.insert_undirected_edge(2, 1);
		sample.insert_undirected_edge(8, 2);
		sample.insert_undirected_edge(7, 1);
		sample.insert_undirected_edge(7, 2);
		sample.insert_undirected_edge(7, 5);
		sample.insert_undirected_edge(7, 6);
		sample.insert_undirected_edge(7, 9);
	}
	else if (alt == 1) {
		sample.insert_undirected_edge(3, 4);
		sample.insert_undirected_edge(4, 8);
		sample.insert_undirected_edge(8, 6);
		sample.insert_undirected_edge(6, 9);
		sample.insert_undirected_edge(9, 5);
		sample.insert_undirected_edge(5, 1);
		sample.insert_undirected_edge(1, 3);
		sample.insert_undirected_edge(4, 2);
		sample.insert_undirected_edge(2, 1);
		sample.insert_undirected_edge(8, 2);
		sample.insert_undirected_edge(7, 1);
		sample.insert_undirected_edge(7, 2);
		sample.insert_undirected_edge(7, 5);
		sample.insert_undirected_edge(3, 5);
		sample.insert_undirected_edge(7, 9);
	}
	else if (alt == 2) {
		sample.insert_undirected_edge(13, 14);
		sample.insert_undirected_edge(14, 18);
		sample.insert_undirected_edge(18, 16);
		sample.insert_undirected_edge(16, 19);
		sample.insert_undirected_edge(19, 15);
		sample.insert_undirected_edge(15, 11);
		sample.insert_undirected_edge(11, 13);
		sample.insert_undirected_edge(14, 12);
		sample.insert_undirected_edge(12, 11);
		sample.insert_undirected_edge(18, 12);
		sample.insert_undirected_edge(17, 11);
		sample.insert_undirected_edge(17, 12);
		sample.insert_undirected_edge(17, 15);
		sample.insert_undirected_edge(13, 15);
		sample.insert_undirected_edge(17, 19);
	}
	else if (alt == 3) {
		sample.insert_undirected_edge(3, 4);
		sample.insert_undirected_edge(4, 8);
		sample.insert_undirected_edge(8, 6);
		sample.insert_undirected_edge(6, 9);
		sample.insert_undirected_edge(9, 5);
		sample.insert_undirected_edge(5, 1);
		sample.insert_undirected_edge(1, 3);
		sample.insert_undirected_edge(4, 2);
		sample.insert_undirected_edge(2, 1);
		sample.insert_undirected_edge(8, 2);
		sample.insert_undirected_edge(7, 1);
		sample.insert_undirected_edge(7, 2);
		sample.insert_undirected_edge(7, 5);
		sample.insert_undirected_edge(7, 6);
	}

	return sample;
}

/**
	Takes in a space or tab separated file and loads it into an NGraph object.

	@param dirPath: The file path of the separated file to load.
	@return: The created graph object.
*/
vector<Graph> CreateGraphsFromDataSetDir(string dirPath, bool printDetails) {
	vector<Graph> retGraphs;
	string token;
	ifstream file;
	long int i = 1;

	for (const auto& entry : fs::directory_iterator(dirPath)) {
		string filePath = entry.path().string();
		file.open(filePath.c_str());

		if (printDetails) {
			cout << "Reading file: " << filePath << "\n";
		}

		if (file.is_open()) {
			Graph g;
			bool isEven = true;
			int x, y;

			while (getline(file, token)) {
				stringstream ss(token);
				while (getline(ss, token, ' ')) {

					// Break if on a line beginning with a % as it's a comment.
					if (token == "%") break;

					if (IsInteger(token)) {
						if (isEven) {
							x = stoi(token);
							isEven = false;
						}
						else {
							y = stoi(token);

							if (printDetails) {
								if (i < 2) {
									cout << "Inserting (" << x << ", " << y << ")\n";
								}
								else {
									cout << '.';
								}
							}

							if (x < y) {
								g.insert_undirected_edge(x, y);
							}
							else {
								g.insert_undirected_edge(y, x);
							}

							++i;
							isEven = true;
						}
					}
				}
			}

			file.close();
			
			if (g.num_nodes() > 0) {
				retGraphs.push_back(g);
			}

			cout << "Done creating Graph, inserted " << g.num_vertices() << " vertices and " << g.num_edges() << " edges.\n";
		}
		else {
			cout << "Could not open file in path: " << filePath << "\n";
		}
	}

	return retGraphs;
}

/**
	Determines if a given string is a valid integer.

	@param s: The string to be considered.
	@return: True if a string can be parsed to an integer.
*/
inline bool IsInteger(const string& s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) {
		return false;
	}

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
}

/**
	Prints out information about the given graph to stdout.

	@param g: The graph to print out.
*/
void PrintGraphDetails(Graph g) {
	cout << "Graph contains:\n\t"
		<< g.num_vertices() << " vertices.\n\t"
		<< g.num_edges() << " edges.\n\t"
		<< g.num_nodes() << " nodes.\n\t";
	
	for (auto p = g.begin(); p != g.end(); p++) {
		cout << "Nodes: " << Graph::node(p) << " ";
	}
	cout << "\n";
}