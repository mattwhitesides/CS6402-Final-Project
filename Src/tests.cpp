// Matt Whitesides
// CS6402 - Adv Data Mining
// 3/19/2019

#define CATCH_CONFIG_MAIN

#include "../Include/catch.hpp"
#include "../Include/preprocess.hpp"
#include "../Include/process.hpp"

using namespace NGraph;

namespace {
   const string _testDirPath = "Data/Datasets/Test";
};

TEST_CASE("Sample Graph Created Correctly", "[NGraph]") {
    cout << "TEST 1: Sample Graph Created Correctly\n";
    
    Graph sample = CreateSampleGraph();

    REQUIRE(sample.degree(7) == 5);
    REQUIRE(sample.num_vertices() == 9);
    REQUIRE(sample.num_edges() == 15);
}

TEST_CASE("Test Graph From File Created Correctly", "[NGraph]") {
    cout << "\nTEST 2: Test Graph From File Created Correctly\n";
    
    vector<Graph> graphs = CreateGraphsFromDataSetDir(_testDirPath, false);

    REQUIRE(graphs.size() == 2);

    REQUIRE(graphs[0].degree(7) == 5);
    REQUIRE(graphs[0].num_vertices() == 9);
    REQUIRE(graphs[0].num_edges() == 15);

    REQUIRE(graphs[1].num_vertices() == 3);
    REQUIRE(graphs[1].num_edges() == 2);
}

TEST_CASE("Detect all single node frequent subgraphs in G", "[NGraph]") {
    cout << "\nTEST 3: Detect all single node frequent subgraphs in G\n";
    
    vector<Graph> graphs = CreateGraphsFromDataSetDir(_testDirPath, false);
    vector<Graph> subgraphs = FrequentOneSubgraphs(graphs, 2);

    REQUIRE(subgraphs.size() == 3);
    for (auto& x : subgraphs) {        
        REQUIRE(x.num_nodes() == 1);
    }
}

TEST_CASE("SubGraphIsomorphism should correctly test isomorphism", "[NGraph]") {
    cout << "\nTEST 4: SubGraphIsomorphism should correctly test isomorphism\n";

    Graph g0 = CreateSampleGraph();
    Graph g0b = CreateSampleGraph();
    Graph g1 = CreateSampleGraph(1);
    Graph g2 = CreateSampleGraph(2);
    Graph g3 = CreateSampleGraph(3);

    REQUIRE(SubGraphIsomorphism(g0, g0b));
    REQUIRE(!SubGraphIsomorphism(g0, g1));
    REQUIRE(!SubGraphIsomorphism(g0, g2));
    REQUIRE(!SubGraphIsomorphism(g0, g3));
}

TEST_CASE("AprioriBased should find frequent subgraphs", "[NGraph]") {
    cout << "\nTEST 5: AprioriBased should find frequent subgraphs\n";

    vector<Graph> graphs = CreateGraphsFromDataSetDir(_testDirPath, false);
    vector<Graph> subGraphs = AprioriBased(graphs, 2, 1);

    REQUIRE(subGraphs.size() == 6);    
}