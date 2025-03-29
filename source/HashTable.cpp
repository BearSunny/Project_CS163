#include "../header/HashTable.h"

int HashTable::getTableSize() {
    return TABLE_SIZE;
}

bool HashTable::isOccupied(int idx) {
    return occ[idx];
}

int HashTable::getKeyAt(int idx) {
    return table[idx];
}

void HashTable::insert(int key) {
    // Linear probing to avoid collision
    int idx = key % TABLE_SIZE;
    while (occ[idx]) idx = (idx + 1) % TABLE_SIZE;
    table[idx] = key;
    occ[idx] = true;
}

bool HashTable::search(int key, int &idx) {
    int cur = key % TABLE_SIZE;

    // No element in current index or the element does not match
    while (occ[cur] && table[cur] != key) { 
        cur = (cur + 1) % TABLE_SIZE;
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
        occ[cur] = false;
        return true;
    }
    
    return false;
}