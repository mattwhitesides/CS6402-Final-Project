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

	string exe = string(argv[0]);
	string directory = string(argv[1]);
	bool isMulti = (exe.find("multi") != string::npos) || (exe.find("CS6402") != string::npos);

	if (argc < 2) {
		cout << "Invalid number of arguments...\n";

		if (isMulti) {
			cout << "Proper usage: >> ./multi.exe {Dataset Directory Path} {Optional Min Support Threshold} {Optional Thread Count} {Optional Output File Name}\n";
			cout << "Ex: " << argv[0] << " \"Data/Test\" 2 2 \"output.txt\"";
		}
		else {
			cout << "Proper usage: >> ./single.exe {Dataset Directory Path} {Optional Min Support Threshold} {Optional Output File Name}\n";
			cout << "Ex: " << argv[0] << " \"Data/Test\" 2 \"output.txt\"";
		}
		return -1;
	}

	int s, t;
	string outFilePath;

	if (isMulti) {
		// Get min support, default to 2.
		s = (argc > 2) ? stoi(argv[2]) : 2;

		// Get thread count, default to 2.
		t = (argc > 3) ? stoi(argv[3]) : 2;

		// Get output file path, default to "subgraphs.txt".
		outFilePath = (argc > 4) ? argv[4] : "subgraphs.txt";
	}
	else {
		s = (argc > 2) ? stoi(argv[2]) : 2;
		outFilePath = (argc > 3) ? argv[3] : "subgraphs.txt";
		t = 1;
	}
	
	cout << "\nSetting minimum support to " << s << endl;
	if (isMulti) cout << "Setting thread count to " << t << endl;
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

	cout << "Found " << frequentSubgraphs.size() << " Frequent Sub-Graphs, writing results to file \"" << outFilePath << "\"...";
	
	// Write out the details of the found frequent subgraphs.
	long int totalVerts = 0;
	long int totalEdges = 0;
	for (auto& g : graphs) {
		totalVerts += g.num_vertices();
		totalEdges += g.num_edges();
	}
	
	ofstream outFile;
	outFile.open(outFilePath);

	outFile << "Loaded graphs from directory, \"" << directory << "\".\n";
	outFile << "Loaded " << graphs.size() << " graphs, containing a total of " << totalVerts << " verticies, and " << totalEdges << " edges.\n";
	outFile << frequentSubgraphs.size() << " Frequent Sub-Graphs found." << endl;
	outFile << "Process took " << durationMin.count() << " minutes, " << (durationSec.count() % 60) << " seconds and " << (durationMil.count() % 1000) << " milliseconds." << endl << endl;

	for (auto& g : frequentSubgraphs) {
		outFile << "Graph contains:\n\t"
			<< g.num_vertices() << " vertices.\n\t"
			<< g.num_edges() << " edges.\n\t"
			<< g.num_nodes() << " nodes.\n"
			<< "\tEdges:\n"
			<< g << "\n";
	}

	outFile.close();

	cout << "Process took " << durationMin.count() << " minutes, " << (durationSec.count() % 60)
		<< " seconds and " << (durationMil.count() % 1000) << " milliseconds." << endl << endl;

	//cout << "Press any key to continue...\n";
	//cin >> s;
	return 0;
}