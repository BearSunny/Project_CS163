#pragma once

#include <vector>
#include <string>

// Declare DEFAULT_LOAD_FACTOR as extern
extern const double DEFAULT_LOAD_FACTOR;

class HashTable {
private:
    int TABLE_SIZE;                 // Size of the hash table
    std::vector<int> table;         // Vector to store keys
    std::vector<bool> occ;          // Vector to track occupied slots

    // Private helper function to compute hash
    int getHash(int key);

public:
    // Constructor and Destructor
    HashTable(int size);
    ~HashTable();

    // Public Methods
    int getTableSize();
    bool isOccupied(int idx);
    int getKeyAt(int idx);
    void insert(int key);
    bool search(int key, int &idx);
    bool remove(int key, int &idx);
    bool loadHashTableFromFile(const std::string& filePath);
};