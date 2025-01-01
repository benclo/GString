#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <set>
#include "Graph.h"

using namespace std;

// Structure for Edit Table Entry
class EditTable {
public:
    int id;                          // Edit Table ID
    vector<vector<int>> entries; // List of [nn, nb, nc, id]

    EditTable(int id);

    // Add an entry to the Edit Table
    void addEntry(const vector<int>& attributes, int graphID);

    // Display the Edit Table (for debugging purposes)
    void display() const;
};

// TrieNode class that stores the structure, children, and Edit Table references
struct TrieNode {
    unordered_map<string, TrieNode*> children; // Maps structure summaries to child nodes
    vector<int> editTableIDs;                      // List of Edit Table IDs

    TrieNode();
};

// Trie class that manages the insertion of paths and Edit Tables
class Trie {
public:
    TrieNode* root;
    vector<EditTable> editTables; // Global list of Edit Tables
    int nextEditTableID;              // ID for the next Edit Table
    vector<int> currentEditTableIDs; // Tracks Edit Table IDs for the current insertion

    Trie();

    // Insert the first suffix (full path) into the Trie
    void pathInsert(const vector<Structure>& path, int graphID);

    // Insert a suffix into the Trie
    void suffixInsert(const vector<Structure>& suffix);

    // Query the Trie with a path
    set<int> query(const vector<Structure>& path, double threshold) const;

    // Display the Trie as connections from root to leaves
    void displayConnections(TrieNode* node, const string& indent = "") const;

    // Display the entire Trie
    void display() const;

    // Display all Edit Tables (for debugging purposes)
    void displayEditTables() const;

    bool isSimilar(const vector<int>& queryAttributes, const vector<int>& tableAttributes, double threshold) const;

    double calculateSimilarity(const vector<int>& queryAttributes, const vector<int>& tableAttributes) const;
};

#endif // TRIE_H
