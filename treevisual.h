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

    int height(Node *p) {
        if(p == nullptr)
            return 0;
        return p->depth;
    }

    Node *rightrotate(Node *p) {
        Node *res = p->left;
        Node *ptr = p->left->right;
        p->left->right = p;
        p->left = ptr;
        res->right->depth = max(height(res->right->left), height(res->right->right)) + 1;
        res->depth = max(height(res->left), height(res->right)) + 1;
        return res;
    }

    Node *leftrotate(Node *p) {
        Node *res = p->right;
        Node *ptr = p->right->left;
        p->right->left = p;
        p->right = ptr;
        res->left->depth = max(height(res->left->left), height(res->left->right)) + 1;
        res->depth = max(height(res->left), height(res->right)) + 1;
        return res;
    }

    Node *insert(Node *p, int x) {
        if(p == nullptr) {
            p = new Node(x);
            return p;
        }
        if(p->data > x) {
            p->left = insert(p->left, x);
            p->depth = max(height(p->left), height(p->right)) + 1;
            if(height(p->left->right) - height(p->right) == 1)
                p->left = leftrotate(p->left);
            if(height(p->left->left) - height(p->right) == 1)
                p = rightrotate(p);
        }
        if(p->data < x) {
            p->right = insert(p->right, x);
            p->depth = max(height(p->left), height(p->right)) + 1;
            if(height(p->right->left) - height(p->left) == 1)
                p->right = rightrotate(p->right);
            if(height(p->right->right) - height(p->left) == 1)
                p = leftrotate(p);
        }
        return p;
    }

    void insert(int x) {
        root = insert(root, x);
    }

    void Draw(Node *p, int d, int x, int y) {
        if(p->left) {
            DrawLine(x, y, x - d, y + 100, BLACK);
            Draw(p->left, d / 2, x - d, y + 100);        
        }
        if(p->right) {
            DrawLine(x, y, x + d, y + 100, BLACK);
            Draw(p->right, d / 2, x + d, y + 100);
        }
        DrawCircle(x, y, 32, DARKGREEN);
        DrawCircle(x, y, 30, GREEN);
    }
} S;

void InsertAVL() {
    if(length == 0)
        return;
    int x = 0;
    for(int i = 0; i < length; ++i)
        x = 10 * x + number[i];
    S.insert(x);
}

void DrawTree() {
    int dist = 35;
    for(int i = 0; i < S.height(S.root) - 2; ++i)
        dist *= 2;
    if(S.root)
        S.Draw(S.root, dist, 500, 100);
}

void DisplayTree() {
    DrawRectangle(0, 0, 1000, 50, GREEN);
    DrawText("AVL TREE", 10, 10, 30, YELLOW);

    DrawRectangle(199, 9, 102, 32, BLACK);
    DrawRectangle(200, 10, 100, 30, WHITE);
    if(InsertButton) {
        framecount = (framecount + 1) % 60;
        DrawText(number, 205, 15, 20, BLACK);
        if(framecount < 30)
            DrawText("|", 205 + MeasureText(number, 20), 15, 20, BLACK);
    }

    DrawRectangle(304, 9, 102, 32, BLACK);
    DrawRectangle(305, 10, 100, 30, LIGHTGRAY);
    DrawText("Insert", 355 - MeasureText("Insert", 20) / 2, 15, 20, BLACK);

    DrawRectangle(409, 9, 102, 32, BLACK);
    DrawRectangle(410, 10, 100, 30, WHITE);
    if(DeleteButton) {
        framecount = (framecount + 1) % 60;
        DrawText(number, 415, 15, 20, BLACK);
        if(framecount < 30)
            DrawText("|", 415 + MeasureText(number, 20), 15, 20, BLACK);
    }

    DrawRectangle(514, 9, 102, 32, BLACK);
    DrawRectangle(515, 10, 100, 30, LIGHTGRAY);
    DrawText("Delete", 565 - MeasureText("Delete", 20) / 2, 15, 20, BLACK);

    DrawRectangle(619, 9, 102, 32, BLACK);
    DrawRectangle(620, 10, 100, 30, WHITE);
    if(FindButton) {
        framecount = (framecount + 1) % 60;
        DrawText(number, 625, 15, 20, BLACK);
        if(framecount < 30)
            DrawText("|", 625 + MeasureText(number, 20), 15, 20, BLACK);
    }

    DrawRectangle(724, 9, 102, 32, BLACK);
    DrawRectangle(725, 10, 100, 30, LIGHTGRAY);
    DrawText("Find", 775 - MeasureText("Find", 20) / 2, 15, 20, BLACK);

    DrawTree();
}

#endif
