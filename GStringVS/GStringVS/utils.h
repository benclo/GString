#ifndef UTILS_H
#define UTILS_H

#include "Graph.h"
#include <string>
#include <vector>

using namespace std;

int extract_graph_id(const string& unified);
vector<Structure> parse_structures(const string& unified);
int extract_graph_id(const string& unified);
Graph simplify_to_graph(const vector<Structure>& structures, int graphID);
vector<vector<Structure>> calculate_suffixes_with_path(const vector<Graph::GraphNode>& nodes, int graphID);

#endif
