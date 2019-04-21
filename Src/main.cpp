// Matt Whitesides
// CS6402 - Adv Data Mining
// 3/19/2019

#include <chrono> 
#include "../Include/preprocess.hpp"
#include "../Include/process.hpp"

using namespace NGraph;
using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {

	if (argc < 2) {
		cout << "Invalid number of arguments...\n";
		cout << "Proper usage: >> ./main.exe {Dataset Directory Path)";
		cout << "Ex: ./main.exe Data/Datasets/Test/out.tsv";
		return -1;
	}


	// Load the graphs in the given directory.
	cout << "Loading dataset...\n";
	vector<Graph> graphs = CreateGraphsFromDataSetDir(argv[1], false);

	// Start a timer.
	auto start = high_resolution_clock::now();

	// Find the frequent subgraphs among the dataset.
	cout << "\nFinding frequent Sub-Graphs...\n";
	auto frequentSubgraphs = AprioriBased(graphs, 2);

	// Stop the timer and get the duration.
	auto stop = high_resolution_clock::now();
	auto durationMil = duration_cast<milliseconds>(stop - start);
	auto durationSec = duration_cast<seconds>(stop - start);
	auto durationMin = duration_cast<minutes>(stop - start);

	// Print out the details of the found frequent subgraphs.
	cout << frequentSubgraphs.size() << " Frequent Sub-Graphs found: " << endl;
	for (auto& g : frequentSubgraphs) {
		PrintGraphDetails(g);
	}
	cout << "Process took " << durationMin.count() << " minutes, " << (durationSec.count() % 60)
		 << " seconds and " << (durationMil.count() % 1000) << " milliseconds." << endl;

	return 0;
}