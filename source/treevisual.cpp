#include "../header/treevisual.h"

AVLTree S;

int AVLTree::height(Node *p) {
    if(p == nullptr)
        return 0;
    return p->depth;
}

AVLTree::Node *AVLTree::rightrotate(Node *p) {
    Node *res = p->left;
    Node *ptr = p->left->right;
    p->left->right = p;
    p->left = ptr;
    res->right->depth = max(height(res->right->left), height(res->right->right)) + 1;
    res->depth = max(height(res->left), height(res->right)) + 1;
    return res;
}

AVLTree::Node *AVLTree::leftrotate(Node *p) {
    Node *res = p->right;
    Node *ptr = p->right->left;
    p->right->left = p;
    p->right = ptr;
    res->left->depth = max(height(res->left->left), height(res->left->right)) + 1;
    res->depth = max(height(res->left), height(res->right)) + 1;
    return res;
}

AVLTree::Node *AVLTree::insert(Node *p, int x) {
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

AVLTree::Node *AVLTree::MaxNode(Node *p) {
    if(p->right == nullptr)
        return p;
    return MaxNode(p->right);
}

AVLTree::Node *AVLTree::remove(Node *p, int x) {
    if(p == nullptr)
        return nullptr;
    if(p->data > x)
        p->left = remove(p->left, x);
    else if(p->data < x)
        p->right = remove(p->right, x);
    else if(p->left == nullptr && p->right == nullptr) {
        delete p;
        p = nullptr;
        return p;
    }
    else if(p->left == nullptr) {
        p->data = p->right->data;
        delete p->right;
        p->right = nullptr;
    }
    else {
        Node *tmp = MaxNode(p->left);
        p->data = tmp->data;
        p->left = remove(p->left, p->data);
    }
    if(p->left != nullptr)
        p->left->depth = max(height(p->left->left), height(p->left->right)) + 1;
    if(p->right != nullptr)
        p->right->depth = max(height(p->right->left), height(p->right->right)) + 1;
    p->depth = max(height(p->left), height(p->right)) + 1;
    if(p->left && height(p->left->right) > height(p->left->left) && height(p->left->right) - height(p->right) == 1)
        p->left = leftrotate(p->left);
    if(p->left && height(p->left->left) - height(p->right) == 1)
        p = rightrotate(p);
    if(p->right && height(p->right->left) > height(p->right->right) && height(p->right->left) - height(p->left) == 1)
        p->right = rightrotate(p->right);
    if(p->right && height(p->right->right) - height(p->left) == 1)
        p = leftrotate(p);
    return p;
}

void AVLTree::clear(Node *&p) {
    if(p->left)
        clear(p->left);
    if(p->right)
        clear(p->right);
    delete p;
    p = nullptr;
}

void AVLTree::UpdatePosition(Node *p, int u, int v) {
    p->newx = u;
    p->newy = v;
    if(p->x == -1) {
        p->x = p->newx;
        p->y = p->newy;
    }
    if(p->depth > 1) {
        int d = 35 * (1 << (p->depth - 2));
        if(p->left)
            UpdatePosition(p->left, u - d, v + 100);
        if(p->right)
            UpdatePosition(p->right, u + d, v + 100);
    }
}

void AVLTree::insert(int x) {
    PushHistory();
    root = insert(root, x);
    UpdatePosition(root, 1000, 100);
}

void AVLTree::remove(int x) {
    PushHistory();
    root = remove(root, x);
    if(root)
        UpdatePosition(root, 1000, 100);
}

void AVLTree::find(int x) {
    queue<Node*> Q;
    Q.emplace(root);
    while(!Q.empty()) {
        Node *p = Q.front();
        Q.pop();
        if(p == nullptr)
            continue;
        if(p->data == x)
            p->findselected = true;
        else
            p->findselected = false;
        Q.emplace(p->left);
        Q.emplace(p->right);
    }
}

void AVLTree::Draw(Node *p) {
    double dx = abs(p->x - p->newx);
    double dy = abs(p->y - p->newy);
    double d = sqrt(dx * dx + dy * dy);
    if(p->x < p->newx) {
        p->x += dx / d;
        if(p->x > p->newx)
            p->x = p->newx;       
    }
    else if(p->x > p->newx) {
        p->x -= dx / d;
        if(p->x < p->newx)
            p->x = p->newx;
    }
    if(p->y < p->newy) {
        p->y += dy / d;
        if(p->y > p->newy)
            p->y = p->newy;       
    }
    else if(p->y > p->newy) {
        p->y -= dy / d;
        if(p->y < p->newy)
            p->y = p->newy;
    }
    if(p->left) {
        DrawLine(p->x, p->y, p->left->x, p->left->y, BLACK);
        Draw(p->left);        
    }
    if(p->right) {
        DrawLine(p->x, p->y, p->right->x, p->right->y, BLACK);
        Draw(p->right);
    }
    if(p->findselected)
        DrawCircle(p->x, p->y, 30, BUTTON);
    else
        DrawCircle(p->x, p->y, 26, BORDER);
    if(p->selected)
        DrawCircle(p->x, p->y, 25, BUTTON);
    else
        DrawCircle(p->x, p->y, 25, HOVERED);
    DrawNumber(p->data, p->x, p->y, 20);
}

bool AVLTree::UpdateSelectedNode(Node *p) {
    if(p == nullptr)
        return false;
    double dx = abs(p->x - GetMousePosition().x);
    double dy = abs(p->y - GetMousePosition().y);
    double d = sqrt(dx * dx + dy * dy);
    if(d <= 25)
        p->selected = true;
    else
        p->selected = false;
    bool res = UpdateSelectedNode(p->left) | UpdateSelectedNode(p->right);    
    return p->selected | res;
}

string AVLTree::FindSelectedNode(Node *p) {
    if(p == nullptr)
        return "";
    if(p->selected)
        return to_string(p->data);
    string res = FindSelectedNode(p->left);
    if(!res.empty())
        return res;
    return FindSelectedNode(p->right);
}

void AVLTree::CopyData(Node *&p, Node *root) {
    if(root == nullptr)
        return;
    p = new Node(root->data);
    p->x = root->x;
    p->y = root->y;
    p->newx = root->newx;
    p->newy = root->newy;
    p->depth = root->depth;
    p->selected = false;
    p->findselected = false;
    CopyData(p->left, root->left);
    CopyData(p->right, root->right);
}

void AVLTree::PushHistory() {
    Node *p = nullptr;
    CopyData(p, root);
    History.emplace(p);
    while(!RedoHistory.empty()) {
        delete RedoHistory.top();
        RedoHistory.pop();
    }
}

void AVLTree::PopHistory() {
    if(History.empty())
        return;
    RedoHistory.emplace(root);
    root = History.top();
    History.pop();
}

void AVLTree::PopRedoHistory() {
    if(RedoHistory.empty())
        return;
    History.emplace(root);
    root = RedoHistory.top();
    RedoHistory.pop();
}

void InsertAVL() {
    if(Number.empty())
        return;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    S.insert(x);
}

void RemoveAVL() {
    if(Number.empty())
        return;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    S.remove(x);
}

void FindAVL() {
    if(Number.empty())
        return;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    S.find(x);
}

void DrawTree() {
    if(S.root)
        S.Draw(S.root);
}

void DrawInitialize() {
    DrawRectangle(545, 220, 510, 460, BLACK);
    DrawRectangle(550, 225, 500, 450, WHITE);

    DrawRectangle(550, 225, 500, 50, GREEN);
    DrawText("Initialize", 800 - MeasureText("Initialize", 30) / 2, 235, 30, BLACK);
    
    DrawRectangle(945, 220, 110, 60, BLACK);
    if(MouseButtonPressed(950, 225, 1050, 275))
        DrawRectangle(950, 225, 100, 50, GRAY);
    else
        DrawRectangle(950, 225, 100, 50, LIGHTGRAY);
    DrawText("Close", 1000 - MeasureText("Close", 30) / 2, 235, 30, BLACK);

    DrawRectangle(545, 270, 510, 110, BLACK);
    if(MouseButtonPressed(550, 275, 1050, 375))
        DrawRectangle(550, 275, 500, 100, GRAY);
    else
        DrawRectangle(550, 275, 500, 100, LIGHTGRAY);
    DrawText("Keyboard", 800 - MeasureText("KeyBoard", 30) / 2, 310, 30, BLACK);

    DrawRectangle(545, 370, 510, 110, BLACK);        
    if(MouseButtonPressed(550, 375, 1050, 475))
        DrawRectangle(550, 375, 500, 100, GRAY);
    else
        DrawRectangle(550, 375, 500, 100, LIGHTGRAY);
    DrawText("File", 800 - MeasureText("File", 30) / 2, 410, 30, BLACK);

    DrawRectangle(545, 470, 510, 110, BLACK);
    if(MouseButtonPressed(550, 475, 1050, 575))
        DrawRectangle(550, 475, 500, 100, GRAY);
    else
        DrawRectangle(550, 475, 500, 100, LIGHTGRAY);
    DrawText("Random", 800 - MeasureText("Random", 30) / 2, 510, 30, BLACK);

    DrawRectangle(545, 570, 510, 110, BLACK);
    if(MouseButtonPressed(550, 575, 1050, 675))
        DrawRectangle(550, 575, 500, 100, GRAY);
    else
        DrawRectangle(550, 575, 500, 100, LIGHTGRAY);
    DrawText("Empty", 800 - MeasureText("Empty", 30) / 2, 610, 30, BLACK);
}

void DrawNumber(int x, int y, int fs) {
    char* c = new char[Number.size() + 1];
    for(int i = 0; i < Number.size(); ++i)
        c[i] = Number[i];
    c[Number.size()] = '\0';
    DrawText(c, x, y, fs, BLACK);
    delete[] c;
}

int MeasureNumber(int Cursor) {
    char *c = new char[Cursor + 1];
    for(int i = 0; i < Cursor; ++i)
        c[i] = Number[i];
    c[Cursor] = '\0';
    return MeasureText(c, 20);
}

void DrawKeyboard() {
    DrawRectangle(295, 395, 1010, 110, BLACK);
    DrawRectangle(300, 400, 1000, 100, WHITE);

    DrawRectangle(300, 400, 1000, 50, GREEN);
    DrawText("Input data (Numbers are seperated by space)", 800 - MeasureText("Input data (Numbers are seperated by space)", 20) / 2, 415, 20, BLACK);
    
    if(MouseButtonPressed(1200, 400, 1300, 450))
        DrawRectangle(1200, 400, 100, 50, GRAY);
    else
        DrawRectangle(1200, 400, 100, 50, LIGHTGRAY);
    DrawText("Close", 1250 - MeasureText("Close", 20) / 2, 415, 20, BLACK);

    if(MouseButtonPressed(1200, 450, 1300, 500))
        DrawRectangle(1200, 450, 100, 50, GRAY);
    else
        DrawRectangle(1200, 450, 100, 50, LIGHTGRAY);
    DrawText("Initialize", 1250 - MeasureText("Initialize", 20) / 2, 465, 20, BLACK);

    DrawLine(1200, 400, 1200, 500, BLACK);
    DrawLine(300, 450, 1300, 450, BLACK);

    DrawNumber(310, 465, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 310 + MeasureNumber(CurrentCursor), 465, 20, BLACK);
}

void DrawFile() {
    DrawRectangle(295, 395, 1010, 110, BLACK);
    DrawRectangle(300, 400, 1000, 100, WHITE);

    DrawRectangle(300, 400, 1000, 50, GREEN);
    DrawText("Copy the path of the file here", 800 - MeasureText("Copy the path of the file", 20) / 2, 415, 20, BLACK);
    
    if(MouseButtonPressed(1200, 400, 1300, 450))
        DrawRectangle(1200, 400, 100, 50, GRAY);
    else
        DrawRectangle(1200, 400, 100, 50, LIGHTGRAY);
    DrawText("Close", 1250 - MeasureText("Close", 20) / 2, 415, 20, BLACK);

    if(MouseButtonPressed(1200, 450, 1300, 500))
        DrawRectangle(1200, 450, 100, 50, GRAY);
    else
        DrawRectangle(1200, 450, 100, 50, LIGHTGRAY);
    DrawText("Initialize", 1250 - MeasureText("Initialize", 20) / 2, 465, 20, BLACK);

    DrawLine(1200, 400, 1200, 500, BLACK);
    DrawLine(300, 450, 1300, 450, BLACK);

    DrawNumber(310, 465, 20);
}

void DrawAdd() {
    DrawNumber(130, 115, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 130 + MeasureNumber(CurrentCursor), 115, 20, BLACK);
}

void DrawDelete() {
    DrawNumber(130, 165, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 130 + MeasureNumber(CurrentCursor), 165, 20, BLACK);
}

void DrawFind() {
    DrawNumber(130, 215, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 130 + MeasureNumber(CurrentCursor), 215, 20, BLACK);
}

void DisplayTree() {
    ClearBackground(BACKGROUND);

    DrawRectangle(9, 9, 212, 42, BORDER);
    DrawRectangle(10, 10, 210, 40, HOVERED);
    DrawText("AVL Tree", 115 - MeasureText("AVL Tree", 30) / 2, 15, 30, TEXT);

    DrawRectangle(9, 59, 102, 32, BORDER);
    if(MouseButtonPressed(10, 60, 110, 90))
        DrawRectangle(10, 60, 100, 30, BUTTON);
    else
        DrawRectangle(10, 60, 100, 30, HOVERED);
    DrawText("Initialize", 60 - MeasureText("Initialize", 20) / 2, 65, 20, TEXT);

    DrawRectangle(9, 109, 102, 32, BORDER);
    if(MouseButtonPressed(10, 110, 110, 140))
        DrawRectangle(10, 110, 100, 30, BUTTON);
    else
        DrawRectangle(10, 110, 100, 30, HOVERED);
    DrawText("Add", 60 - MeasureText("Add", 20) / 2, 115, 20, TEXT);

    DrawRectangle(119, 109, 102, 32, BORDER);
    DrawRectangle(120, 110, 100, 30, WHITE);

    DrawRectangle(9, 159, 102, 32, BORDER);
    if(MouseButtonPressed(10, 160, 110, 190))
        DrawRectangle(10, 160, 100, 30, BUTTON);
    else
        DrawRectangle(10, 160, 100, 30, HOVERED);
    DrawText("Delete", 60 - MeasureText("Delete", 20) / 2, 165, 20, TEXT);

    DrawRectangle(119, 159, 102, 32, BORDER);
    DrawRectangle(120, 160, 100, 30, WHITE);

    DrawRectangle(9, 209, 102, 32, BORDER);
    if(MouseButtonPressed(10, 210, 110, 240))
        DrawRectangle(10, 210, 100, 30, BUTTON);
    else
        DrawRectangle(10, 210, 100, 30, HOVERED);
    DrawText("Find", 60 - MeasureText("Find", 20) / 2, 215, 20, TEXT);

    DrawRectangle(119, 209, 102, 32, BORDER);
    DrawRectangle(120, 210, 100, 30, WHITE);

    DrawRectangle(9, 259, 102, 32, BORDER);
    if(MouseButtonPressed(10, 260, 110, 290))
        DrawRectangle(10, 260, 100, 30, BUTTON);
    else
        DrawRectangle(10, 260, 100, 30, HOVERED);
    DrawText("Undo", 60 - MeasureText("Undo", 20) / 2, 265, 20, TEXT);

    DrawRectangle(119, 259, 102, 32, BORDER);
    if(MouseButtonPressed(120, 260, 220, 290))
        DrawRectangle(120, 260, 100, 30, BUTTON);
    else
        DrawRectangle(120, 260, 100, 30, HOVERED);
    DrawText("Redo", 170 - MeasureText("Redo", 20) / 2, 265, 20, TEXT);

    DrawTree();

    if(CurrentButton == INITIALIZEBUTTON)
        DrawInitialize();
    else if(CurrentButton == KEYBOARDBUTTON)
        DrawKeyboard();
    else if(CurrentButton == FILEBUTTON)
        DrawFile();
    else if(CurrentButton == ADDBUTTON)
        DrawAdd();
    else if(CurrentButton == DELETEBUTTON)
        DrawDelete();
    else if(CurrentButton == FINDBUTTON)
        DrawFind();
}

void UpdateNumber(bool NumberOnly) {
    if(IsKeyPressed(KEY_ZERO)) {
        Number.insert(Number.begin() + CurrentCursor, '0');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_ONE)) {
        Number.insert(Number.begin() + CurrentCursor, '1');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_TWO)) {
        Number.insert(Number.begin() + CurrentCursor, '2');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_THREE)) {
        Number.insert(Number.begin() + CurrentCursor, '3');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_FOUR)) {
        Number.insert(Number.begin() + CurrentCursor, '4');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_FIVE)) {
        Number.insert(Number.begin() + CurrentCursor, '5');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_SIX)) {
        Number.insert(Number.begin() + CurrentCursor, '6');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_SEVEN)) {
        Number.insert(Number.begin() + CurrentCursor, '7');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_EIGHT)) {
        Number.insert(Number.begin() + CurrentCursor, '8');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_NINE)) {
        Number.insert(Number.begin() + CurrentCursor, '9');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_SPACE) && !NumberOnly) {
        Number.insert(Number.begin() + CurrentCursor, ' ');
        ++CurrentCursor;
    }
    else if(IsKeyPressed(KEY_BACKSPACE) && CurrentCursor) {
        Number.erase(Number.begin() + CurrentCursor - 1);
        --CurrentCursor;
    }
    else if(IsKeyPressed(KEY_LEFT) && CurrentCursor)
        --CurrentCursor;
    else if(IsKeyPressed(KEY_RIGHT) && CurrentCursor < Number.size())
        ++CurrentCursor;
}

void InitializeKeyboard() {
    if(S.root)
        S.clear(S.root);
    int N = 0;
    for(int i = 0; i < Number.size(); ++i) {
        if(Number[i] == ' ') {
            if(i && Number[i - 1] != ' ')
                S.insert(N);
            N = 0;
        }
        else
            N = 10 * N + Number[i] - '0';
    }
    if(!Number.empty() && Number.back() != ' ')
        S.insert(N);
}

void UpdatePath() {
    if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V))
        Number = GetClipboardText();
}

void InitializeFile() {
    if(S.root)
        S.clear(S.root);
    ifstream f(Number);
    string s;
    while(getline(f, s)) {
        int N = 0;
        for(int i = 0; i < s.size(); ++i) {
            if(s[i] == ' ') {
                if(i && s[i - 1] != ' ')
                    S.insert(N);
                N = 0;
            }
            else
                N = 10 * N + s[i] - '0';
        }
        if(s.back() != ' ')
            S.insert(N);
    }
}

void InitializeRandom() {
    if(S.root)
        S.clear(S.root);
    for(int i = 0; i < 20; ++i)
        S.insert(rand(0, 99));
}

void InitializeEmpty() {
    if(S.root)
        S.clear(S.root);
}

void TREE_INTERACT() {
    if(CurrentButton == NULLBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 110, 220, 140)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 160, 220, 190)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 160, 110, 190)) {
                Number = S.FindSelectedNode(S.root);
                RemoveAVL();
            }
            else if(MouseButtonPressed(120, 210, 220, 240)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 260, 110, 290))
                S.PopHistory();
            else if(MouseButtonPressed(120, 260, 220, 290))
                S.PopRedoHistory();
            S.UpdateSelectedNode(S.root);
        }
    }
    else if(CurrentButton == INITIALIZEBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(950, 225, 1050, 275))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(550, 275, 1050, 375)) {
                CurrentButton = KEYBOARDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(550, 375, 1050, 475)) {
                CurrentButton = FILEBUTTON;
                Number.clear();
            }
            else if(MouseButtonPressed(550, 475, 1050, 575)) {
                InitializeRandom();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(550, 575, 1050, 675)) {
                InitializeEmpty();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 110, 220, 140)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 160, 220, 190)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 210, 220, 240)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 260, 110, 290)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 260, 220, 290)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
        }
    }
    else if(CurrentButton == KEYBOARDBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(1200, 400, 1300, 450))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(1200, 450, 1300, 500)) {
                InitializeKeyboard();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 110, 220, 140)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 160, 220, 190)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 210, 220, 240)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 260, 110, 290)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 260, 220, 290)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
        }
        else
            UpdateNumber(false);
    }
    else if(CurrentButton == FILEBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(1200, 400, 1300, 450))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(1200, 450, 1300, 500)) {
                InitializeFile();
                CurrentButton = NULLBUTTON;
            }
            if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 110, 220, 140)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 160, 220, 190)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 210, 220, 240)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 260, 110, 290)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 260, 220, 290)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
        }
        else
            UpdatePath();
    }
    else if(CurrentButton == ADDBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(10, 110, 110, 140)) {
                InsertAVL();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 160, 220, 190)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(S.UpdateSelectedNode(S.root))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(120, 210, 220, 240)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 260, 110, 290)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 260, 220, 290)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
        }
        else
            UpdateNumber(true);
    }
    else if(CurrentButton == DELETEBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(10, 160, 110, 190)) {
                RemoveAVL();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 110, 220, 140)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(S.UpdateSelectedNode(S.root))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(120, 210, 220, 240)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 260, 110, 290)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 260, 220, 290)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
        }
        else
            UpdateNumber(true);
    }
    else if(CurrentButton == FINDBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if(MouseButtonPressed(10, 210, 110, 240)) {
                FindAVL();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 110, 220, 140)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 160, 220, 190)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(S.UpdateSelectedNode(S.root))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(10, 260, 110, 290)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 260, 220, 290)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
        }
        else
            UpdateNumber(true);
    }
}
