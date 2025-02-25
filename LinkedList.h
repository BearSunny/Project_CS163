#ifndef LINKEDLIST_H
#define LINKEDLIST_H

struct Node {
    int val;
    Node* next;
    Node(int x) : val(x), next(nullptr) {}
};

class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    
    Node* getHead();
    int getSize();
    
    void add(int x);           // Add at end
    void insertAt(int idx, int x); // Add at index
    void update(int idx, int x);   // Update value at index
    bool deleteAt(int idx);        // Delete at index
    int getAt(int idx);           // Get value at index
    int search(int x);            // Search for value
    void reset();                 // Reset to initial state
    void clear();                 // Clear entire list

private:
    Node* head;
};

#endif 