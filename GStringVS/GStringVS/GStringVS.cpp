#include "Graph.h"
#include "utils.h"
#include "Trie.h"
#include <iostream>

int main() {
    string unified = "g1 Line 2 <node 1 O><node 2 O><edge 1 2 d><c 2 3>"
        "Cycle 5 <node 2 O><node 3 F><edge 2 3 d><edge 3 5 d><edge 5 2 d><c 3 1>"
        "Star 4 <node 0 S><node 2 O><node 3 O><node 4 O><edge 0 3 d><edge 0 4 d><branch 1 C>";

    int graphID = extract_graph_id(unified);
    size_t pos = unified.find(" ") + 1;
    string structures_string = unified.substr(pos);

    vector<Structure> structures = parse_structures(structures_string);
    Graph graph = simplify_to_graph(structures, graphID);
    Graph rGraph = graph.reverse_graph();
    vector<vector<Structure>> suffixes = calculate_suffixes_with_path(graph.nodes, graphID);
    vector<vector<Structure>> rSuffixes = calculate_suffixes_with_path(rGraph.nodes, graphID);

    Trie trie;

    trie.pathInsert(suffixes[0], graphID); // Insert full path
    for (size_t i = 1; i < suffixes.size(); ++i) {
        trie.suffixInsert(suffixes[i]); // Insert suffixes
    }

    trie.pathInsert(rSuffixes[0], graphID); // Insert full path
    for (size_t i = 1; i < rSuffixes.size(); ++i) {
        trie.suffixInsert(rSuffixes[i]); // Insert suffixes
    }

    string unified2 = "g2 Line 2 <node 1 O><node 2 O><edge 1 2 d><c 2 3>"
        "Cycle 5 <node 2 O><node 3 F><edge 2 3 d><edge 3 5 d><edge 5 2 d><c 3 1>"
        "Star 3 <node 0 S><node 2 O><node 3 O><node 4 O><edge 0 3 d><edge 0 4 d><branch 1 C>";

    int graphID2 = extract_graph_id(unified2);
    size_t pos2 = unified2.find(" ") + 1;
    string structures_string2 = unified2.substr(pos2);

    vector<Structure> structures2 = parse_structures(structures_string2);
    Graph graph2 = simplify_to_graph(structures2, graphID2);
    Graph rGraph2 = graph2.reverse_graph();
    vector<vector<Structure>> suffixes2 = calculate_suffixes_with_path(graph2.nodes, graphID2);
    vector<vector<Structure>> rSuffixes2 = calculate_suffixes_with_path(rGraph2.nodes, graphID2);

    trie.pathInsert(suffixes2[0], graphID2); // Insert full path
    for (size_t i = 1; i < suffixes2.size(); ++i) {
        trie.suffixInsert(suffixes2[i]); // Insert suffixes
    }

    trie.pathInsert(rSuffixes2[0], graphID2); // Insert full path
    for (size_t i = 1; i < rSuffixes2.size(); ++i) {
        trie.suffixInsert(rSuffixes2[i]); // Insert suffixes
    }

    trie.display();

    cout << "\nEdits Table:\n";
    trie.displayEditTables();

    // Example query path
    vector<Structure> queryPath = rSuffixes[1]; // Replace with the actual query path

    // Query the Trie
    set<int> result = trie.query(queryPath);

    // Display the results
    cout << "Query Results:\n";
    for (int graphID : result) {
        cout << "Graph ID: " << graphID << endl;
    }

    return 0;
}
