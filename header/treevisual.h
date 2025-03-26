#ifndef TREE_H
#define TREE_H

#include "declare.h"

struct AVLTree {
    struct Node {
        int x, y, newx, newy;
        int data, depth;
        Node *left, *right;

        Node(int d) {
            x = -1;
            y = -1;
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
    void UpdatePosition(Node *p, int u, int v);
    void insert(int x);
    void remove(int x);
    bool find(int x);
    void Draw(Node *p);
};

void InsertAVL();
void RemoveAVL();
void FindAVL();
void DrawTree();
void DisplayTree();
void TREE_INTERACT();

#endif
