#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <vector>
#include <string>
#include <fstream>
using std::vector;
using std::string;
using std::ifstream;

struct Node {
    int val;
    Node* next;
    
    Node(int x) : val(x), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    
    // Helper method to create the default list
    void createDefaultList();

public:
    LinkedList();
    LinkedList(bool empty);
    ~LinkedList();
    
    void clear();
    Node* getHead() const;
    int getSize();
    // Create list methods
    void createFromValues(const std::vector<int>& values);
    bool createFromFile(const std::string& filePath);
    
    // Add methods
    void add(int x);
    void addMultiple(const std::vector<int>& values);
    void insertAt(int idx, int x);
    
    // Update and delete methods
    void update(int idx, int x);
    bool deleteAt(int idx);
    
    // Get and search methods
    int getAt(int idx);
    Node* getNodeAt(int idx);
    int search(int x);
    
    // Reset method
    void reset();
};

#endif // LINKEDLIST_H
