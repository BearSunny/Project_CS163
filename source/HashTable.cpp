#include "../header/HashTable.h"

#include <fstream>
#include <sstream>


#include "HashTable.h"
#include <fstream>
#include <sstream>

// Define DEFAULT_LOAD_FACTOR here
const double DEFAULT_LOAD_FACTOR = 0.75;

// Private helper function to compute hash
int HashTable::getHash(int key) {
    return key % TABLE_SIZE;
}

// Constructor
HashTable::HashTable(int size) : TABLE_SIZE(size) {
    table.resize(size);
    occ.resize(size, 0);
}

// Destructor
HashTable::~HashTable() {
    table.clear();
    occ.clear();
}



int HashTable::getTableSize() {
    return TABLE_SIZE;
}

int HashTable::isOccupied(int idx) {
    return occ[idx];
}

int HashTable::getKeyAt(int idx) {
    return table[idx];
}

bool HashTable::insert(int key, int &idx) {
    int cnt = 0;
    if (search(key, idx)) return false;
    // Linear probing to avoid collision
    idx = key % TABLE_SIZE;
    while (occ[idx] == 1) {
        idx = (idx + 1) % TABLE_SIZE;
        cnt++;
        if (cnt == TABLE_SIZE) return false;
    }
    table[idx] = key;
    occ[idx] = 1;

    return true;
}

bool HashTable::search(int key, int &idx) {
    int cur = key % TABLE_SIZE;
    int cnt = 0;    

    // No element in current index or the element does not match
    while (occ[cur] && table[cur] != key) { 
        cur = (cur + 1) % TABLE_SIZE;
        cnt++;
        if (cnt == TABLE_SIZE) return false;
    }
    if (occ[cur] && table[cur] == key) {
        idx = cur;
        return true;
    }
    
    return false;
}

bool HashTable::remove(int key, int &idx) {
    int cur = key % TABLE_SIZE;

    // No element in current index or the element does not match
    while (occ[cur] && table[cur] != key) { 
        cur = (cur + 1) % TABLE_SIZE;
    }
    if (occ[cur] && table[cur] == key) {
        idx = cur;
        occ[cur] = 2;
        return true;
    }
    
    return false;
}

void HashTable::update(int newKey) {
    int idx = newKey % TABLE_SIZE;

    occ[idx] = 1;
    table[idx] = newKey;

    return;
}

bool HashTable::loadHashTableFromFile(const std::string& filePath) {
    std::ifstream input(filePath);
    if (!input.is_open()) {
        return false; // Không mở được file
    }

    int newSize;
    input >> newSize;
    if (input.fail() || newSize <= 0 || newSize > 60) {
        input.close();
        return false; // Không đọc được kích thước hợp lệ
    }

    // Cập nhật kích thước bảng và làm mới các vector lưu trữ
    TABLE_SIZE = newSize;
    table.assign(newSize, 0);
    occ.assign(newSize, 0);

    int key, idx = -1;
    // Đọc các key từ file và chèn vào bảng
    while (input >> key) {
        insert(key, idx);
    }

    input.close();
    return true;
}