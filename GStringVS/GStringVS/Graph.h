#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

using namespace std;

struct Node {
    int id;
    string property;
};

struct Edge {
    int from;
    int to;
    string property;
};

struct Conn {
    int from;
    int to;
};

struct Structure {
    string type;   // "Line", "Cycle", "Star", etc.
    int size = 0;  // Size of the structure (e.g., number of edges/nodes)
    vector<Node> nodes;
    vector<Edge> edges;
    vector<string> branches;
    vector<Conn> conn;

    // Calculate the [nn, nb, ne] values for the structure
    string summary_string() const;

    vector<int> getAttributes() const;

};

class Graph {
public:
    int graphID;  // Store the graph identifier
    struct GraphNode {
        Structure data;  // Store the actual Structure
    };

    struct GraphEdge {
        const Structure* from; // Pointer to the source structure
        const Structure* to;   // Pointer to the target structure
    };

    vector<GraphNode> nodes; // List of graph nodes
    vector<GraphEdge> edges; // List of graph edges

    // Constructor to set the graph ID
    Graph(int id);

    // Add a node to the graph
    void add_node(const Structure& structure);

    // Add an edge to the graph
    void add_edge(const Structure* from, const Structure* to);

    // Reverse the graph and return the reversed graph
    Graph reverse_graph() const;

    // Display the graph
    void display() const;
};

#endif
