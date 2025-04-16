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

typedef enum ButtonState { NULLBUTTON, INITIALIZEBUTTON, KEYBOARDBUTTON, FILEBUTTON, EMPTYBUTTON, RANDOMBUTTON, ADDBUTTON, DELETEBUTTON, FINDBUTTON, ADDSBS, DELETESBS } ButtonState;
extern ButtonState CurrentButton;

extern std::mt19937_64 rng;
extern std::string Number;
extern int CurrentCursor;
extern bool STEPBYSTEPBUTTON;

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
    std::stack<Node *> History;
    std::stack<Node *> RedoHistory;
    std::queue<std::tuple<Node *, Node *, int>> Q;
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
    void UpdateDepth(Node *p);
    void UpdatePosition(Node *p, int u, int v);
    void insert(int x);
    void insertSBS(int x);
    bool insertStepByStep();
    void remove(int x);
    void removeSBS(int x);
    bool removeStepByStep();
    void find(int x);
    void Draw(Node *p);
    bool UpdateSelectedNode(Node *p);
    std::string FindSelectedNode(Node *p);
    void CopyData(Node *&p, Node *root);
    void PushHistory();
    void PopHistory();
    void PopRedoHistory();
};

void InsertAVL();
void InsertAVLSBS();
void RemoveAVL();
void RemoveAVLSBS();
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