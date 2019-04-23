// Matt Whitesides
// CS6402 - Adv Data Mining
// 3/19/2019

#include <chrono> 
#include "../Include/preprocess.hpp"
#include "../Include/multi_process.hpp"

using namespace NGraph;
using namespace std;
using namespace std::chrono;

int main(int argc, char** argv) {

	if (argc < 2) {
		cout << "Invalid number of arguments...\n";
		cout << "Proper usage: >> ./multi.exe {Dataset Directory Path} {Optional Min Support Threshold} {Optional Thread Count} {Optional Output File Name}\n";
		cout << "Ex: " << argv[0] << " \"Data/Test\" 2 2 \"output.txt\"";
		return -1;
	}

	// Get min support, default to 2.
	int s = (argc > 2) ? stoi(argv[2]) : 2;
	cout << "\nSetting minimum support to " << s << endl;

	int t = (argc > 3) ? stoi(argv[3]) : 1;
	cout << "Setting thread count to " << t << endl;

	string outFilePath = (argc > 4) ? argv[4] : "subgraphs.txt";
	cout << "Setting outputfile to \"" << outFilePath << "\"\n\n";

	// Load the graphs in the given directory.
	cout << "Loading dataset...\n";
	vector<Graph> graphs = CreateGraphsFromDataSetDir(argv[1], false);

	// Start a timer.
	auto start = high_resolution_clock::now();

	// Find the frequent subgraphs among the dataset.
	cout << "\nFinding frequent Sub-Graphs...\n\n";
	auto frequentSubgraphs = AprioriBased(graphs, s, t);

	// Stop the timer and get the duration.
	auto stop = high_resolution_clock::now();
	auto durationMil = duration_cast<milliseconds>(stop - start);
	auto durationSec = duration_cast<seconds>(stop - start);
	auto durationMin = duration_cast<minutes>(stop - start);

	// Write out the details of the found frequent subgraphs.
	cout << "Found Frequent Sub-Graphs writing results to file \"" << outFilePath << "\"...";
	ofstream outFile;
	outFile.open(outFilePath);
	outFile << frequentSubgraphs.size() << " Frequent Sub-Graphs found." << endl;
	for (auto& g : frequentSubgraphs) {
		outFile << "Graph contains:\n\t"
			<< g.num_vertices() << " vertices.\n\t"
			<< g.num_edges() << " edges.\n\t"
			<< g.num_nodes() << " nodes.\n"
			<< "\tEdges:\n"
			<< g << "\n";
	}
	outFile.close();
	cout << "Done.\n";

	cout << frequentSubgraphs.size() << " Frequent Sub-Graphs found." << endl;
	cout << "Process took " << durationMin.count() << " minutes, " << (durationSec.count() % 60)
		<< " seconds and " << (durationMil.count() % 1000) << " milliseconds." << endl << endl;

	cout << "Press any key to continue...\n";
	cin >> s;
	return 0;
}
