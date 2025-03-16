#pragma once

#include <iostream>

struct Node {
    int key;
    int value;
    Node* pNext;

    Node(int item1, int item2) {
        key = item1;
        value = item2;
        pNext = nullptr;
    }
};

const double DEFAULT_LOAD_FACTOR = 0.75;

class HashTable {
private:
    int TABLE_SIZE;
    int currentSize;
    Node** table;
    
    bool isPrime(int n) {
        if (n < 2) return false;
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    int getNextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

    int getHash(int key) {
        return key % TABLE_SIZE;
    }

public:
    HashTable() : TABLE_SIZE(19), currentSize(0) {
        table = new Node*[TABLE_SIZE]{nullptr};
    }

    HashTable(int size) : currentSize(0) {
        TABLE_SIZE = getNextPrime(2 * size);
        table = new Node*[TABLE_SIZE]{nullptr};
    }
    
    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* cur = table[i];
            while (cur) {
                Node* trash = cur;
                cur = cur->pNext;
                delete trash;
            }
        }
        delete[] table;
    }

    void insert(int key, int value);
    bool find(int key, int &val);
    void remove(int key);
    void print();
    void rehashing();
};