#include "Trie.h"
#include <iostream>
#include <algorithm>
#include <iterator>
#include <set>

// Constructor for EditTable
EditTable::EditTable(int id) : id(id) {}

// Add an entry to the Edit Table
void EditTable::addEntry(const vector<int>& attributes, int graphID) {
    vector<int> entry = attributes;
    entry.push_back(graphID); // Add the graph ID to the attributes

    // Check if the entry already exists
    if (find(entries.begin(), entries.end(), entry) == entries.end()) {
        entries.push_back(entry); // Add only if it's unique
    }
}

// Display the Edit Table (for debugging purposes)
void EditTable::display() const {
    cout << "Edit Table ID " << id << ":\n";
    for (const auto& entry : entries) {
        cout << "[";
        for (size_t i = 0; i < entry.size(); ++i) {
            cout << entry[i];
            if (i < entry.size() - 1) cout << ", ";
        }
        cout << "]\n";
    }
}

// Constructor for TrieNode
TrieNode::TrieNode() {}

// Constructor for Trie
Trie::Trie() : root(new TrieNode()), nextEditTableID(0) {}

// Insert the first suffix (full path) into the Trie
void Trie::pathInsert(const vector<Structure>& path, int graphID) {
    TrieNode* node = root; // Start from the root
    currentEditTableIDs.clear(); // Clear previous Edit Table IDs

    for (const auto& structure : path) {
        string structure_summary = structure.summary_string(); // Get the structure summary

        // Check if the structure already exists as a child
        if (node->children.find(structure_summary) == node->children.end()) {
            // If not, create a new node
            node->children[structure_summary] = new TrieNode();

            // Create a new Edit Table for the node
            EditTable newEditTable(nextEditTableID++);
            newEditTable.addEntry(structure.getAttributes(), graphID); // Add [nn, nb, nc, id]
            editTables.push_back(newEditTable); // Add the table to the global list

            // Associate the Edit Table with this node
            node->children[structure_summary]->editTableIDs.push_back(newEditTable.id);

            // Add the new Edit Table ID to currentEditTableIDs
            currentEditTableIDs.push_back(newEditTable.id);
        }
        else {
            // If the structure already exists, find its first associated Edit Table
            TrieNode* existingNode = node->children[structure_summary];
            int editTableID = existingNode->editTableIDs[0]; // First Edit Table ID
            editTables[editTableID].addEntry(structure.getAttributes(), graphID); // Add entry to the existing Edit Table

            // Add the existing Edit Table ID to currentEditTableIDs
            currentEditTableIDs.push_back(editTableID);
        }

        // Move to the existing or newly created child node
        node = node->children[structure_summary];
    }
}

// Insert a suffix into the Trie
void Trie::suffixInsert(const vector<Structure>& suffix) {
    TrieNode* node = root; // Start from the root

    size_t offset = currentEditTableIDs.size() - suffix.size(); // Start index in currentEditTableIDs

    for (size_t i = 0; i < suffix.size(); ++i) {
        const auto& structure = suffix[i];
        string structure_summary = structure.summary_string(); // Get the structure summary

        // Check if the structure already exists as a child
        if (node->children.find(structure_summary) == node->children.end()) {
            // If not, create a new node
            node->children[structure_summary] = new TrieNode();

            // Associate the existing Edit Table ID from currentEditTableIDs
            node->children[structure_summary]->editTableIDs.push_back(currentEditTableIDs[offset + i]);
        }
        else {
            // If the structure already exists, add the existing Edit Table ID
            TrieNode* existingNode = node->children[structure_summary];
            int editTableID = currentEditTableIDs[offset + i];

            // Check if the Edit Table ID is already present before adding
            if (find(existingNode->editTableIDs.begin(), existingNode->editTableIDs.end(), editTableID) == existingNode->editTableIDs.end()) {
                existingNode->editTableIDs.push_back(editTableID);
            }
        }

        // Move to the existing or newly created child node
        node = node->children[structure_summary];
    }
}

// Query the Trie with a path
set<int> Trie::query(const vector<Structure>& path) const {
    TrieNode* node = root; // Start from the root
    set<int> result;       // Graph IDs matching the query path

    for (const auto& structure : path) {
        string structure_summary = structure.summary_string(); // Get the structure summary

        // Check if the structure exists as a child
        if (node->children.find(structure_summary) == node->children.end()) {
            // If not found, return an empty set (no match)
            return {};
        }

        // Move to the matching child node
        node = node->children.at(structure_summary);

        // Gather matching graph IDs from the Edit Tables of this node
        set<int> currentIDs;
        for (int tableID : node->editTableIDs) {
            for (const auto& entry : editTables[tableID].entries) {
                currentIDs.insert(entry.back()); // Graph ID is the last element
            }
        }

        // Intersect with the result set
        if (result.empty()) {
            result = currentIDs; // Initialize with the first set of IDs
        }
        else {
            set<int> intersection;
            set_intersection(result.begin(), result.end(), currentIDs.begin(), currentIDs.end(),
                inserter(intersection, intersection.begin()));
            result = intersection;
        }

        // If the intersection becomes empty, stop early
        if (result.empty()) return {};
    }

    return result;
}

// Display the Trie as connections from root to leaves
void Trie::displayConnections(TrieNode* node, const string& indent) const {
    for (const auto& child : node->children) {
        cout << indent << child.first; // Print the current node (structure summary)

        // Display the associated Edit Tables
        if (!child.second->editTableIDs.empty()) {
            cout << " [Edit Tables: ";
            for (size_t i = 0; i < child.second->editTableIDs.size(); ++i) {
                cout << child.second->editTableIDs[i];
                if (i < child.second->editTableIDs.size() - 1) cout << ", ";
            }
            cout << "]";
        }

        // If the child has no further children, mark it as END
        if (child.second->children.empty()) {
            cout << " -> END";
        }

        cout << endl;

        // Recursively display connections for the child
        displayConnections(child.second, indent + "  ");
    }
}

// Display the entire Trie
void Trie::display() const {
    displayConnections(root); // Start from the root
}

// Display all Edit Tables (for debugging purposes)
void Trie::displayEditTables() const {
    for (const auto& table : editTables) {
        table.display();
    }
}
