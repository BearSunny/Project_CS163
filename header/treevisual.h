#ifndef TREE_H
#define TREE_H

#include "declare.h"
#include <stack>
#include <queue>
#include <tuple>
#include <random>
#include <string>

extern const Color BACKGROUND;
extern const Color BUTTON;
extern const Color HOVERED;
extern const Color BORDER;
extern const Color TEXT;

const string ADDSTEPBYSTEP[] = {
    "Node* insert(Node *p, int x) {",
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
    const string DELETESTEPBYSTEP[] = {
    "Node* remove(Node *p, int x) {",
    "    if(p == nullptr) return nullptr;",
    "    if(p->data > x) p->left = remove(p->left, x);",
    "    else if(p->data < x) p->right = remove(p->right, x);",
    "    else if(p->left == nullptr && p->right == nullptr) {",
    "        delete p; p = nullptr;",
    "        return p;",
    "    }",
    "    else if(p->left == nullptr) {",
    "        p->data = p->right->data;",
    "        delete p->right; p->right = nullptr;",
    "    }",
    "    else {",
    "        Node *tmp = MaxNode(p->left);",
    "        p->data = tmp->data;",
    "        p->left = remove(p->left, p->data);",
    "    }",
    "    p->depth = max(height(p->left), height(p->right)) + 1;",
    "    if(p->left && height(p->left->right) > height(p->left->left) && height(p->left->right) - height(p->right) == 1) p->left = leftrotate(p->left);",
    "    if(p->left && height(p->left->left) - height(p->right) == 1) p = rightrotate(p);",
    "    if(p->right && height(p->right->left) > height(p->right->right) && height(p->right->left) - height(p->left) == 1) p->right = rightrotate(p->right);",
    "    if(p->right && height(p->right->right) - height(p->left) == 1) p = leftrotate(p);",
    "    return p;",
    "}"};
    const pair<pair<int, string>, string> DELETESTEPBYSTEP2[] = {
    {{0, ""}, "Node* remove(Node *p, int x) {"},
    {{1, "    "}, "if(p == nullptr)"},
    {{1, "    if(p == nullptr) "}, "return nullptr;"},
    {{2, "    "}, "if(p->data > x)"},
    {{2, "    if(p->data > x) "}, "p->left = remove(p->left, x);"},
    {{3, "    "}, "else if(p->data < x)"},
    {{3, "    else if(p->data < x) "}, "p->right = remove(p->right, x);"},
    {{4, "    "}, "else if(p->left == nullptr && p->right == nullptr) {"},
    {{5, "        "}, "delete p; p = nullptr;"},
    {{6, "        "}, "return p;"},
    {{7, ""}, "    }"},
    {{8, "    "}, "else if(p->left == nullptr) {"},
    {{9, "        "}, "p->data = p->right->data;"},
    {{10, "        "}, "delete p->right; p->right = nullptr;"},
    {{11, ""}, "    }"},
    {{12, ""}, "    else {"},
    {{13, "        "}, "Node *tmp = MaxNode(p->left);"},
    {{14, "        "}, "p->data = tmp->data;"},
    {{15, "        "}, "p->left = remove(p->left, p->data);"},
    {{16, ""}, "    }"},
    {{17, "    "}, "p->depth = max(height(p->left), height(p->right)) + 1;"},
    {{18, "    "}, "if(p->left && height(p->left->right) > height(p->left->left) && height(p->left->right) - height(p->right) == 1)"},
    {{18, "    if(p->left && height(p->left->right) > height(p->left->left) && height(p->left->right) - height(p->right) == 1) "}, "p->left = leftrotate(p->left);"},
    {{19, "    "}, "if(p->left && height(p->left->left) - height(p->right) == 1)"},
    {{19, "    if(p->left && height(p->left->left) - height(p->right) == 1) "}, "p = rightrotate(p);"},
    {{20, "    "}, "if(p->right && height(p->right->left) > height(p->right->right) && height(p->right->left) - height(p->left) == 1)"},
    {{20, "    if(p->right && height(p->right->left) > height(p->right->right) && height(p->right->left) - height(p->left) == 1) "}, "p->right = rightrotate(p->right);"},
    {{21, "    "}, "if(p->right && height(p->right->right) - height(p->left) == 1)"},
    {{21, "    if(p->right && height(p->right->right) - height(p->left) == 1) "}, "p = leftrotate(p);"},
    {{22, "    "}, "return p;"},
    {{23, ""}, "}"}};
    const string FINDSTEPBYSTEP[] {
    "bool find(int x) {",
    "    Node* p = root;",
    "    while(p != nullptr) {",
    "        if(p->data == x)",
    "            return true;",
    "        if(p->data > x)",
    "            p = p->left;",
    "        else",
    "            p = p->right;",
    "    }",
    "    return false;",
    "}"};

typedef enum ButtonState { NULLBUTTON, INITIALIZEBUTTON, KEYBOARDBUTTON, FILEBUTTON, EMPTYBUTTON, RANDOMBUTTON, ADDBUTTON, DELETEBUTTON, FINDBUTTON, ADDSBS, DELETESBS, FINDSBS } ButtonState;
extern ButtonState CurrentButton;

extern std::mt19937_64 rng;
extern std::string Number;
extern int CurrentCursor;
extern bool STEPBYSTEPBUTTON;

extern int CheckFind;
extern int rootx;
extern int rooty;
extern int oldx;
extern int oldy;
extern bool Pressed;
extern int InsertNumber;
extern int query;
extern float Speed;
extern int Startx;
extern int Starty;

struct AVLTree {
    struct Node {
        double x, y, newx, newy;
        int data, depth;
        Node *left, *right;
        bool selected;
        bool findselected;

        Node(int d);
    };

    Node *root = nullptr;
    stack<Node*> History;
    stack<Node*> RedoHistory;
    queue<tuple<Node*, Node*, int>> Q;
    Node *prep;

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
    std::string FindSelectedNode(Node *p);
    void CopyData(Node *&p, Node *root);
    void PushHistory();
    void PopHistory();
    void PopRedoHistory();
};

void InsertAVL();
bool InsertAVLSBS();
void RemoveAVL();
bool RemoveAVLSBS();
void FindAVL();
bool FindAVLSBS();
void DrawTree();
void DrawInitialize();
void DrawNumber(int x, int y, int fs);
int MeasureNumber(int Cursor);
void DrawKeyboard();
void DrawFile();
void DrawAdd();
void DrawDelete();
void DrawAddSBS();
void DrawDeleteSBS();
void DrawFindSBS();
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