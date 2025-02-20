#include <iostream>
#include <bits/stdc++.h>
#include "DummyLinkedList.h"
using namespace std;

DummyLinkedList::DummyLinkedList() {
    head = new Node(10);
    Node* node1 = new Node(20);
    Node* node2 = new Node(30);
    Node* node3 = new Node(40);
    Node* node4 = new Node(50);

    head -> next = node1;
    node1 -> next = node2;
    node2 -> next = node3;
    node3 -> next = node4;
}

DummyLinkedList::~DummyLinkedList() {
    Node* cur = head;
    while (cur) {
        Node* nxt = cur -> next;
        delete cur;
        cur = nxt;
    }
}

Node* DummyLinkedList::getHead() {
    return head;
}

// Add at the back
void DummyLinkedList::add(int x) {
    if (!head) {
        head = new Node(x);
    } else {
        Node* cur = head;
        while (cur -> next) {
            cur = cur -> next;
        }
        cur -> next = new Node(x);
    }
}

void DummyLinkedList::update(int idx, int x) {
    if (idx < 0) return;
    Node* cur = head;
    int curIdx = 0;

    while (cur && curIdx < idx) {
        cur = cur -> next;
        curIdx++;
    }

    if (cur != nullptr) {
        cur -> val = x;
    }
}

void DummyLinkedList::deleteAt(int idx) {
    Node* cur = head;
    int curIdx = 0;

    while (cur && curIdx < idx) {
        cur = cur -> next;
        curIdx++;
    }

    Node* temp = cur -> next;
    delete cur;
    cur = temp;
}
