#pragma once

#include "HashTable.h";

void HashTable::insert(int key, int value) {
    int index = getHash(key);

    Node* cur = table[index];
    while (cur) {
        if (cur->key == key) {
            cur->value = value;
            return;
        }
        cur = cur->pNext;
    }

    Node* newNode = new Node(key, value);
    newNode->pNext = table[index];
    table[index] = newNode;

    currentSize++;
    double loadFactor = (double)currentSize / TABLE_SIZE;
    if (loadFactor > DEFAULT_LOAD_FACTOR) {
        std::cout << "Load factor is greater than " << DEFAULT_LOAD_FACTOR << std::endl;
        std::cout << "Therefore Rehashing will be done." << std::endl;
        rehashing();
        std::cout << "New Size of Hash Table: " << TABLE_SIZE << std::endl;
    }
}

bool HashTable::find(int key, int &val) {
    int index = getHash(key);
    Node* cur = table[index];
    while (cur) {
        if (cur->key == key) {
            val = cur->value;
            return true;
        }
        cur = cur->pNext;
    }

    return false;
}

void HashTable::remove(int key) {
    int index = getHash(key);
    if (!table[index]) return;
    
    Node* cur = table[index], *prev = nullptr;
    while (cur && cur->key != key) {
        prev = cur;
        cur = cur->pNext;
    }

    if (!cur) return; 

    if (prev == nullptr) {
        table[index] = cur->pNext;
    } else {
        prev->pNext = cur->pNext;
    }

    delete cur;
}

void HashTable::print() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (!table[i]) continue;

        Node* cur = table[i];
        while(cur) {
            std::cout << "{" << cur->key << "," << cur->value << "} "; 
            cur = cur->pNext;
        }
        std::cout << std::endl;
    }
}

void HashTable::rehashing() {
    std::cout << "Rehashing Started\n" << std::endl;

    // Tăng kích thước bảng, chọn số nguyên tố lớn hơn TABLE_SIZE*2
    int NEW_TABLE_SIZE = TABLE_SIZE * 2 + 1;
    Node** newTable = new Node*[NEW_TABLE_SIZE]{nullptr};

    // Chuyển dữ liệu từ bảng cũ sang bảng mới
    for (int i = 0; i < TABLE_SIZE; i++) {
        Node* cur = table[i];
        while (cur) {
            int newIndex = cur->key % NEW_TABLE_SIZE;
            Node* next = cur->pNext;

            // Chèn vào bảng mới
            cur->pNext = newTable[newIndex];
            newTable[newIndex] = cur;

            cur = next;
        }
    }

    // Xoá bảng cũ
    delete[] table;

    // Cập nhật bảng và kích thước mới
    table = newTable;
    TABLE_SIZE = NEW_TABLE_SIZE;
}