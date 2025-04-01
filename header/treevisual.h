#ifndef TREE_H
#define TREE_H

#include "declare.h"

const Color BACKGROUND = {241, 231, 231, 255};
const Color BUTTON = {77, 85, 204, 255};
const Color HOVERED = {245, 162, 178, 255};
const Color BORDER = {194, 24, 91, 255};
const Color TEXT = {255, 254, 206, 255};

typedef enum ButtonState{NULLBUTTON, INITIALIZEBUTTON, KEYBOARDBUTTON, FILEBUTTON, EMPTYBUTTON, RANDOMBUTTON, ADDBUTTON, DELETEBUTTON, FINDBUTTON} ButtonState;
ButtonState CurrentButton = NULLBUTTON;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
long long rand(long long l, long long r) {return rng() % (r - l + 1) + l;}

string Number;
int CurrentCursor = 0;

struct AVLTree {
    struct Node {
        double x, y, newx, newy;
        int data, depth;
        Node *left, *right;
        bool selected;
        bool findselected;

        Node(int d) {
            x = -1;
            y = -1;
            data = d;
            depth = 1;
            left = nullptr;
            right = nullptr;
            selected = false;
            findselected = false;
        }
    };

    Node *root = nullptr;
    stack<Node*> History;
    stack<Node*> RedoHistory;

    int height(Node *p);
    Node *rightrotate(Node *p);
    Node *leftrotate(Node *p);
    Node *insert(Node *p, int x);
    Node *MaxNode(Node *p);
    Node *remove(Node *p, int x);
    void clear(Node *&p);
    void UpdatePosition(Node *p, int u, int v);
    void insert(int x);
    void remove(int x);
    void find(int x);
    void Draw(Node *p);
    bool UpdateSelectedNode(Node *p);
    string FindSelectedNode(Node *p);
    void CopyData(Node *&p, Node *root);
    void PushHistory();
    void PopHistory();
    void PopRedoHistory();
};

void InsertAVL();
void RemoveAVL();
void FindAVL();
void DrawTree();
void DrawInitialize();
void DrawNumber(int x, int y, int fs);
int MeasureNumber(int Cursor);
void DrawKeyboard();
void DrawFile();
void DrawAdd();
void DrawDelete();
void DisplayTree();
void UpdateNumber();
void InitializeKeyboard();
void UpdatePath();
void InitializeFile();
void InitializeRandom();
void InitializeEmpty();
void TREE_INTERACT();

#endif
