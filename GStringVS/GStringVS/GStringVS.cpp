#include "Graph.h"
#include "utils.h"
#include "Trie.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <chrono>

// Helper function to generate a random number within a range
int random_int(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}

// Function to generate a Line structure
std::pair<std::string, int> generate_line() {
    int size = random_int(2, 5); // Line size (minimum 2 vertices)
    std::ostringstream line;
    line << "Line " << size << " ";
    for (int i = 1; i <= size; ++i) {
        line << "<node " << i << " " << (random_int(0, 2) == 0 ? 'O' : (random_int(0, 1) == 0 ? 'N' : 'C')) << ">";
    }
    for (int i = 1; i < size; ++i) {
        line << "<edge " << i << " " << (i + 1) << " " << (random_int(0, 1) == 0 ? 's' : 'd') << ">";
    }
    return { line.str(), size };
}

// Function to generate a Cycle structure
std::pair<std::string, int> generate_cycle() {
    int size = random_int(3, 6); // Cycle size (minimum 3 vertices)
    std::ostringstream cycle;
    cycle << "Cycle " << size << " ";
    for (int i = 1; i <= size; ++i) {
        cycle << "<node " << i << " " << (random_int(0, 2) == 0 ? 'C' : (random_int(0, 1) == 0 ? 'O' : 'F')) << ">";
    }
    for (int i = 1; i <= size; ++i) {
        int next = (i == size) ? 1 : i + 1;
        cycle << "<edge " << i << " " << next << " " << (random_int(0, 1) == 0 ? 's' : 'd') << ">";
    }
    return { cycle.str(), size };
}

// Function to generate a Star structure
std::pair<std::string, int> generate_star() {
    int fanout = random_int(3, 5); // Fanout (number of branches)
    std::ostringstream star;
    star << "Star " << fanout << " ";
    star << "<node 0 S>";
    for (int i = 1; i <= fanout; ++i) {
        star << "<node " << i << " " << (random_int(0, 1) == 0 ? 'O' : 'N') << ">";
    }
    for (int i = 1; i <= fanout; ++i) {
        star << "<edge 0 " << i << " " << (random_int(0, 1) == 0 ? 's' : 'd') << ">";
    }
    star << "<branch " << random_int(1, fanout) << " C>";
    return { star.str(), fanout };
}

// Function to randomly pick a structure type
std::pair<std::string, int> generate_random_structure() {
    int choice = random_int(0, 2);
    if (choice == 0) return generate_line();
    if (choice == 1) return generate_cycle();
    return generate_star();
}

// Function to generate a single GString with connections
std::string generate_gstring(int g_id) {
    int num_structures = random_int(10, 15); // Random number of structures per GString
    std::ostringstream gstring;
    gstring << "g" << g_id << " "; // Use to_string for the ID
    int last_node = random_int(1, 5); // Random starting node for connections

    for (int i = 0; i < num_structures; ++i) {
        auto [structure, size] = generate_random_structure();
        gstring << structure;
        if (i < num_structures - 1) { // Add a connection if it's not the last structure
            gstring << "<c " << last_node << " " << random_int(1, size) << ">";
            last_node = random_int(1, size); // Update last node for the next connection
        }
    }
    return gstring.str();
}

int main() {
    srand(std::time(nullptr)); // Seed the random number generator

    std::chrono::time_point<std::chrono::system_clock> start, end;

    start = std::chrono::system_clock::now();

    Trie trie;

    int graphQ = 500;

    vector<Structure> queryPath;

    for (int i = 0; i < graphQ; ++i)
    {
        //string unified = "g1 Line 2 <node 1 O><node 2 O><edge 1 2 d><c 2 3>Cycle 5 <node 2 O><node 3 F><edge 2 3 d><edge 3 5 d><edge 5 2 d><c 3 1>Star 4 <node 0 S><node 2 O><node 3 O><node 4 O><edge 0 3 d><edge 0 4 d><branch 1 C>";
        string unified = generate_gstring(i);
        int graphID = extract_graph_id(unified);
        size_t pos = unified.find(" ") + 1;
        string structures_string = unified.substr(pos);

        vector<Structure> structures = parse_structures(structures_string);
        Graph graph = simplify_to_graph(structures, graphID);
        Graph rGraph = graph.reverse_graph();
        vector<vector<Structure>> suffixes = calculate_suffixes_with_path(graph.nodes, graphID);
        vector<vector<Structure>> rSuffixes = calculate_suffixes_with_path(rGraph.nodes, graphID);

        if (i == 1)
            queryPath = rSuffixes[1];

        trie.pathInsert(suffixes[0], graphID); // Insert full path
        for (size_t i = 1; i < suffixes.size(); ++i) {
            trie.suffixInsert(suffixes[i]); // Insert suffixes
        }

        trie.pathInsert(rSuffixes[0], graphID); // Insert full path
        for (size_t i = 1; i < rSuffixes.size(); ++i) {
            trie.suffixInsert(rSuffixes[i]); // Insert suffixes
        }
    }

    end = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Indexing time with " << graphQ << " graphes: " << elapsed_seconds.count() << "s\n";

    start = std::chrono::system_clock::now();

    double threshold = 0.8; // Set matching threshold (0.8 = allow some differences)
    std::set<int> matchingGraphs = trie.query(queryPath, threshold);

    end = std::chrono::system_clock::now();

    elapsed_seconds = end - start;

    std::cout << "Querying time with " << graphQ << " graphes: " << elapsed_seconds.count() << "s\n";

    // Process the resulting matching graph IDs
    for (int graphID : matchingGraphs)
        std::cout << "Matching Graph ID: " << graphID << std::endl;

    return 0;
}
