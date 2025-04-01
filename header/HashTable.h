#pragma once

#include <vector>

const double DEFAULT_LOAD_FACTOR = 0.75;

class HashTable {
private:
    int TABLE_SIZE;
    std::vector<int> table;
    std::vector<bool> occ;
    
    int getHash(int key) {
        return key % TABLE_SIZE;
    }

public:
    HashTable(int size) : TABLE_SIZE(size) {
        table.resize(size);
        occ.resize(size, false);
    }
    
    ~HashTable() {
        table.clear();
        occ.clear();
    }

    int getTableSize();
    bool isOccupied(int idx);
    int getKeyAt(int idx);

    void insert(int key);

    bool search(int key, int &idx);

    bool remove(int key, int &idx);

    // void rehashing();

    bool loadHashTableFromFile(const std::string& filePath);
};