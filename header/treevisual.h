#ifndef TREE_H
#define TREE_H

#include "declare.h"

struct AVLTree {
    struct Node {
        int data, depth;
        Node *left, *right;

        Node(int d) {
            data = d;
            depth = 1;
            left = nullptr;
            right = nullptr;
        }
    };

    Node *root = nullptr;
    int height(Node *p);
    Node *rightrotate(Node *p);
    Node *leftrotate(Node *p);
    Node *insert(Node *p, int x);
    Node *MaxNode(Node *p);
    Node *remove(Node *p, int x);
    void insert(int x);
    void remove(int x);
    bool find(int x);
    void Draw(Node *p, int d, int x, int y);
} S;

void InsertAVL();
void RemoveAVL();
void FindAVL();
void DrawTree();
void DisplayTree();
void TREE_INTERACT();

#endif
