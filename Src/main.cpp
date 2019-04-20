// Matt Whitesides
// CS6402 - Adv Data Mining
// 3/19/2019

#include "../Include/preprocess.hpp"
#include "../Include/process.hpp"

using namespace NGraph;
using namespace std;

int main(int argc, char** argv) {

	if (argc < 2) {
		cout << "Invalid number of arguments...\n";
		cout << "Proper usage: >> ./main.exe {Dataset File Path)";
		cout << "Ex: ./main.exe Data/Datasets/Test/out.tsv";
		return -1;
	}

	cout << "Loading dataset...\n";

	// Load the given dataset into a graph object.
	vector<Graph> graphs = CreateGraphsFromDataSetDir(argv[1]);

	Graph g0 = CreateSampleGraph();
	Graph g0b = CreateSampleGraph();
	Graph g1 = CreateSampleGraph(1);
	Graph g2 = CreateSampleGraph(2);
	Graph g3 = CreateSampleGraph(3);

	auto r1 = !SubGraphIsomorphism(g0, g1);

	cin >> argc;
	return 0;
}