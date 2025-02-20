// DummyLinkedList.h
#ifndef DUMMYLINKEDLIST_H
#define DUMMYLINKEDLIST_H

struct Node {
    int val;
    Node* next;
    Node(int x) : val(x), next(nullptr) {}
};

class DummyLinkedList {
public:
    DummyLinkedList();
    ~DummyLinkedList();

    Node* getHead();   
    // Dummy add function
    void add(int x);
    // Other dummy methods if needed.
    void update(int idx, int x);
    void deleteAt(int idx);
    
private:
    Node* head;
};

#endif
