#include "../header/HashTable.h"

#include <fstream>
#include <sstream>

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
    int idx = -1, cnt = 0;
    if (search(key, idx)) return;
    // Linear probing to avoid collision
    idx = key % TABLE_SIZE;
    while (occ[idx]) {
        idx = (idx + 1) % TABLE_SIZE;
        cnt++;
        if (cnt == TABLE_SIZE) return;
    }
    table[idx] = key;
    occ[idx] = true;
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
        occ[cur] = false;
        return true;
    }
    
    return false;
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
    occ.assign(newSize, false);

    int key;
    // Đọc các key từ file và chèn vào bảng
    while (input >> key) {
        insert(key);
    }

    input.close();
    return true;
}