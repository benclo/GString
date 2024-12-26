#include "Graph.h"
#include <iostream>

string Structure::summary_string() const {
    return type + " " + to_string(size) + " [" + to_string(nodes.size()) + ", " +
        to_string(branches.size()) + ", " + to_string(edges.size()) + "]";
}

vector<int> Structure::getAttributes() const {
    return { static_cast<int>(nodes.size()),
             static_cast<int>(branches.size()),
             static_cast<int>(edges.size()) };
}

Graph::Graph(int id) : graphID(id) {}

void Graph::add_node(const Structure& structure) {
    nodes.push_back({ structure });
}

void Graph::add_edge(const Structure* from, const Structure* to) {
    edges.push_back({ from, to });
}

Graph Graph::reverse_graph() const {
    Graph reversed_graph(graphID);  // Preserve the original graph ID
    for (size_t i = nodes.size(); i-- > 0;) {
        reversed_graph.add_node(nodes[i].data);
    }

    for (size_t i = edges.size(); i-- > 0;) {
        reversed_graph.add_edge(edges[i].to, edges[i].from);
    }

    return reversed_graph;
}

void Graph::display() const {
    for (size_t i = 0; i < edges.size(); ++i) {
        cout << edges[i].from->summary_string();
        if (i == edges.size() - 1) {
            cout << ", " << edges[i].to->summary_string();
        }
        else {
            cout << ", ";
        }
    }
    cout << "\n";
}
