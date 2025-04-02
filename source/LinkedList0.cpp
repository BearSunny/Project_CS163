#ifndef LINKEDLIST0_H
#define LINKEDLIST0_H

#include <iostream>
#include <vector>
#include <fstream>
#include "header/LinkedList.h"
using namespace std;

// Default constructor - empty linked list
LinkedList::LinkedList() {
    head = nullptr;
}

// Constructor with empty flag - creates an empty list if true
LinkedList::LinkedList(bool empty) {
    head = nullptr;
    if (!empty) {
        createDefaultList();
    }
}

LinkedList::~LinkedList() {
    clear();
}

// Helper method to create the default list
void LinkedList::createDefaultList() {
    clear(); // Clear any existing nodes
    
    head = new Node(10);
    Node* node1 = new Node(20);
    Node* node2 = new Node(30);
    Node* node3 = new Node(40);
    Node* node4 = new Node(50);

    head->next = node1;
    node1->next = node2;
    node2->next = node3;
    node3->next = node4;
}

// Create a list from a vector of values
void LinkedList::createFromValues(const std::vector<int>& values) {
    clear(); // Clear any existing nodes
    
    if (values.empty()) {
        return; // Nothing to add
    }
    
    head = new Node(values[0]);
    Node* current = head;
    
    for (size_t i = 1; i < values.size(); i++) {
        current->next = new Node(values[i]);
        current = current->next;
    }
}

// Create a list from a file
bool LinkedList::createFromFile(const std::string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        return false;
    }
    
    std::vector<int> values;
    int value;
    
    while (file >> value) {
        values.push_back(value);
    }
    
    file.close();
    
    if (values.empty()) {
        return false; // No valid integers found
    }
    
    createFromValues(values);
    return true;
}

void LinkedList::clear() {
    Node* cur = head;
    while (cur) {
        Node* nxt = cur->next;
        delete cur;
        cur = nxt;
    }
    head = nullptr;
}

Node* LinkedList::getHead() const {
    return head;
}

int LinkedList::getSize() {
    int len = 0;
    Node* cur = head;
    while (cur != nullptr) {
        cur = cur->next;
        len += 1;
    }
    return len;
}

// Add at the back
void LinkedList::add(int x) {
    if (!head) {
        head = new Node(x);
        return;
    }
    
    Node* cur = head;
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = new Node(x);
}

void LinkedList::addAtHead(int x) {
    Node* newNode = new Node(x);
    newNode->next = head;
    head = newNode;
}

// Add multiple values at once
void LinkedList::addMultiple(const std::vector<int>& values) {
    for (int value : values) {
        add(value);
    }
}

// Add at specific index
void LinkedList::insertAt(int idx, int x) {
    if (idx < 0) return;
    
    if (idx == 0) {
        Node* newNode = new Node(x);
        newNode->next = head;
        head = newNode;
        return;
    }

    Node* cur = head;
    int curIdx = 0;
    
    while (cur && curIdx < idx - 1) {
        cur = cur->next;
        curIdx++;
    }

    if (cur) {
        Node* newNode = new Node(x);
        newNode->next = cur->next;
        cur->next = newNode;
    }
}

void LinkedList::update(int idx, int x) {
    if (idx < 0) return;
    
    Node* cur = head;
    int curIdx = 0;

    while (cur && curIdx < idx) {
        cur = cur->next;
        curIdx++;
    }

    if (cur) {
        cur->val = x;
    }
}

bool LinkedList::deleteAt(int idx) {
    if (idx < 0 || !head) return false;

    if (idx == 0) {
        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    Node* cur = head;
    Node* prev = nullptr;
    int curIdx = 0;

    while (cur && curIdx < idx) {
        prev = cur;
        cur = cur->next;
        curIdx++;
    }

    if (cur) {
        prev->next = cur->next;
        delete cur;
        return true;
    }

    return false;
}

// Get value at index
int LinkedList::getAt(int idx) {
    if (idx < 0) return -1;
    
    Node* cur = head;
    int curIdx = 0;

    while (cur && curIdx < idx) {
        cur = cur->next;
        curIdx++;
    }

    return cur ? cur->val : -1;
}

// Get node at index
Node* LinkedList::getNodeAt(int idx) {
    if (idx < 0) return nullptr;
    
    Node* cur = head;
    int curIdx = 0;

    while (cur && curIdx < idx) {
        cur = cur->next;
        curIdx++;
    }

    return cur;
}

// Search for a value and return its index
int LinkedList::search(int x) {
    Node* cur = head;
    int idx = 0;

    while (cur) {
        if (cur->val == x) {
            return idx;
        }
        cur = cur->next;
        idx++;
    }

    return -1;
}

// Reset list to initial state
void LinkedList::reset() {
    clear();
    createDefaultList();
}

#endif // LINKEDLIST0_H
