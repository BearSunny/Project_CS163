#ifndef TREE_H
#define TREE_H

#include "declare.h"

const Color BACKGROUND = {241, 231, 231, 255};
const Color BUTTON = {77, 85, 204, 255};
const Color HOVERED = {245, 162, 178, 255};
const Color BORDER = {194, 24, 91, 255};
const Color TEXT = {255, 254, 206, 255};
const string ADDSTEPBYSTEP[] = {"Node* insert(Node *p, int x) {",
                                "    if(p == nullptr) {",
                                "        p = new Node(x);",
                                "        return p;",
                                "    }",
                                "    if(p->data > x) {",
                                "        p->left = insert(p->left, x);",
                                "        p->depth = max(height(p->left), height(p->right)) + 1;",
                                "        if(height(p->left->right) - height(p->right) == 1)",
                                "            p->left = leftrotate(p->left);",
                                "        if(height(p->left->left) - height(p->right) == 1)",
                                "            p = rightrotate(p);",
                                "    }",
                                "    else if(p->data < x) {",
                                "        p->right = insert(p->right, x);",
                                "        p->depth = max(height(p->left), height(p->right)) + 1;",
                                "        if(height(p->right->left) - height(p->left) == 1)",
                                "            p->right = rightrotate(p->right);",
                                "        if(height(p->right->right) - height(p->left) == 1)",
                                "            p = leftrotate(p);",
                                "    }",
                                "    return p;",
                                "}"};

typedef enum ButtonState{NULLBUTTON, INITIALIZEBUTTON, KEYBOARDBUTTON, FILEBUTTON, EMPTYBUTTON, RANDOMBUTTON, ADDBUTTON, DELETEBUTTON, FINDBUTTON, ADDSBS, DELETESBS, FINDSBS} ButtonState;
ButtonState CurrentButton = NULLBUTTON;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
long long rand(long long l, long long r) {return rng() % (r - l + 1) + l;}

string Number;
int CurrentCursor = 0;
bool STEPBYSTEPBUTTON = false;
int CheckFind = 0;
int rootx = 1000;
int rooty = 100;
int oldx = 0;
int oldy = 0;
bool Pressed = false;
int InsertNumber = 0;
int query = -1;

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
    queue<tuple<Node*, Node*, int>> Q;
    Node *prep = nullptr;

    int height(Node *p);
    Node *rightrotate(Node *p);
    Node *leftrotate(Node *p);
    Node *insert(Node *p, int x);
    Node *insertSBS(Node *parent, Node *prep, Node *p, int x);
    Node *MaxNode(Node *p);
    Node *remove(Node *p, int x);
    Node *removeSBS(Node *parent, Node *prep, Node *p, int x);
    void clear(Node *&p);
    void ResetFindSelected();
    void UpdateDepth(Node *p);
    void UpdatePosition(Node *p, int u, int v);
    void UpdatePosition2(Node *p, int u, int v);
    void insert(int x);
    void insertSBS(int x);
    bool insertStepByStep();
    void remove(int x);
    void removeSBS(int x);
    bool removeStepByStep();
    bool find(int x);
    void findSBS(int x);
    bool findStepByStep();
    void Draw(Node *p);
    bool UpdateSelectedNode(Node *p);
    string FindSelectedNode(Node *p);
    void CopyData(Node *&p, Node *root);
    void PushHistory();
    void PopHistory();
    void PopRedoHistory();
} S;

void InsertAVL();
bool InsertAVLSBS();
void RemoveAVL();
void RemoveAVLSBS();
void FindAVL();
void FindAVLSBS();
void DrawTree();
void DrawInitialize();
void DrawNumber(int x, int y, int fs);
int MeasureNumber(int Cursor);
void DrawKeyboard();
void DrawFile();
void DrawAdd();
void DrawDelete();
void DrawAddSBS();
void DisplayTree();
void UpdateNumber();
void InitializeKeyboard();
void UpdatePath();
void InitializeFile();
void InitializeRandom();
void InitializeEmpty();
void BackToMainMenu();
void TREE_INTERACT();

#endif
