#include "utils.h"
#include <sstream>
#include <regex>
#include <cctype>

int extract_int(const string& str, size_t& pos) {
    size_t end_pos = str.find_first_not_of("0123456789", pos);
    int value = stoi(str.substr(pos, end_pos - pos));
    pos = (end_pos == string::npos) ? end_pos : end_pos + 1; // Update position
    return value;
}

vector<Structure> parse_structures(const string& unified) {
    vector<Structure> structures;
    size_t pos = 0;
    while (pos < unified.length()) {
        if (isspace(unified[pos])) {
            ++pos;
            continue;
        }

        Structure structure;
        if (unified.substr(pos, 4) == "Line" || unified.substr(pos, 5) == "Cycle" || unified.substr(pos, 4) == "Star") {
            structure.type = unified.substr(pos, unified.find_first_of(" \t", pos) - pos);
            pos = unified.find_first_of(" \t", pos) + 1;
            structure.size = extract_int(unified, pos);
        }

        while (unified.substr(pos, 5) == "<node") {
            pos += 6;
            Node node;
            node.id = extract_int(unified, pos);
            size_t start = unified.find_first_not_of(" \t", pos);
            size_t end = unified.find_first_of(" >", start);
            node.property = unified.substr(start, end - start);
            pos = end + 1;
            structure.nodes.push_back(node);
        }

        while (unified.substr(pos, 5) == "<edge") {
            pos += 6;
            Edge edge;
            edge.from = extract_int(unified, pos);
            edge.to = extract_int(unified, pos);
            size_t start = unified.find_first_not_of(" \t", pos);
            size_t end = unified.find_first_of(" >", start);
            edge.property = unified.substr(start, end - start);
            pos = end + 1;
            structure.edges.push_back(edge);
        }

        while (unified.substr(pos, 7) == "<branch") {
            pos += 8;
            int branch_id = extract_int(unified, pos);
            size_t start = unified.find_first_not_of(" \t", pos);
            size_t end = unified.find_first_of(" >", start);
            std::string branch_property = unified.substr(start, end - start);
            pos = end + 1;
            structure.branches.push_back(branch_property);
        }

        while (unified.substr(pos, 2) == "<c") {
            pos += 3;
            Conn conn;
            conn.from = extract_int(unified, pos);
            conn.to = extract_int(unified, pos);
            structure.conn.push_back(conn);
        }

        structures.push_back(structure);
    }

    return structures;
}

int extract_graph_id(const std::string& unified) {
    std::regex graphRegex("(g\\d+)\\s*");
    std::smatch match;
    if (std::regex_search(unified, match, graphRegex)) {
        std::string graphIDStr = match[1].str();
        return std::stoi(graphIDStr.substr(1));  // Remove the 'g' and convert to integer
    }
    return -1;
}

// Simplify structures into a graph
Graph simplify_to_graph(const vector<Structure>& structures, int graphID) {
    Graph graph(graphID);

    for (const auto& structure : structures) {
        graph.add_node(structure);
    }

    for (size_t i = 0; i < structures.size(); ++i) {
        for (const auto& connection : structures[i].conn) {
            graph.add_edge(&structures[i], &structures[i + 1]); // Now safe
        }
    }

    return graph;
}

// Calculate suffixes, including the full path
vector<vector<Structure>> calculate_suffixes_with_path(const vector<Graph::GraphNode>& nodes, int graphID) {
    vector<vector<Structure>> suffixes;

    // Save the full path as the first suffix
    vector<Structure> full_path;
    for (const auto& node : nodes) {
        full_path.push_back(node.data);
    }
    suffixes.push_back(full_path);

    // Generate the rest of the suffixes
    for (size_t i = 1; i < nodes.size(); ++i) {
        vector<Structure> suffix;
        for (size_t j = i; j < nodes.size(); ++j) {
            suffix.push_back(nodes[j].data);
        }
        suffixes.push_back(suffix);
    }

    return suffixes;
}