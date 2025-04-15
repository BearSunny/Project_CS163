#include "../header/treevisual.h"

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
    else if(p->data < x) {
        p->right = insert(p->right, x);
        p->depth = max(height(p->left), height(p->right)) + 1;
        if(height(p->right->left) - height(p->left) == 1)
            p->right = rightrotate(p->right);
        if(height(p->right->right) - height(p->left) == 1)
            p = leftrotate(p);
    }
    return p;
}

AVLTree::Node *AVLTree::insertSBS(Node *parent, Node *prep, Node *p, int x) {
    Q.emplace(parent, prep, 1);
    if(p == nullptr) {
        Q.emplace(parent, prep, 2);
        p = new Node(x);
        Q.emplace(parent, prep, 3);
        return p;
    }
    bool check = false;
    Q.emplace(parent, prep, 5);
    if(p->data > x) {
        check = true;
        Q.emplace(parent, prep, 6);
        p->left = insertSBS(prep, prep->left, p->left, x);
        Q.emplace(parent, prep, 7);
        p->depth = max(height(p->left), height(p->right)) + 1;
        Q.emplace(parent, prep, 8);
        if(height(p->left->right) - height(p->right) == 1) {
            Q.emplace(parent, prep, 9);
            p->left = leftrotate(p->left);
        }
        Q.emplace(parent, prep, 10);
        if(height(p->left->left) - height(p->right) == 1) {
            Q.emplace(parent, prep, 11);
            p = rightrotate(p);
        }
    }
    if(check == false)
        Q.emplace(parent, prep, 13);
    if(p->data < x) {
        Q.emplace(parent, prep, 14);
        p->right = insertSBS(prep, prep->right, p->right, x);
        Q.emplace(parent, prep, 15);
        p->depth = max(height(p->left), height(p->right)) + 1;
        Q.emplace(parent, prep, 16);
        if(height(p->right->left) - height(p->left) == 1) {
            Q.emplace(parent, prep, 17);
            p->right = rightrotate(p->right);
        }
        Q.emplace(parent, prep, 18);
        if(height(p->right->right) - height(p->left) == 1) {
            Q.emplace(parent, prep, 19);
            p = leftrotate(p);
        }
    }
    Q.emplace(parent, prep, 21);
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

AVLTree::Node *AVLTree::removeSBS(Node *parent, Node *prep, Node *p, int x) {
    bool check = false;
    Q.emplace(parent, prep, 1);
    if(p == nullptr) {
        check = true;
        Q.emplace(parent, prep, 2);
        return nullptr;
    }
    if(!check) {
        Q.emplace(parent, prep, 3);
        if(p->data > x) {
            check = true;
            Q.emplace(parent, prep, 4);
            p->left = removeSBS(prep, prep->left, p->left, x);
        }
    }
    if(!check) {
        Q.emplace(parent, prep, 5);
        if(p->data < x) {
            check = true;
            Q.emplace(parent, prep, 6);
            p->right = removeSBS(prep, prep->right, p->right, x);
        }
    }
    if(!check) {
        Q.emplace(parent, prep, 7);
        if(p->left == nullptr && p->right == nullptr) {
            check = true;
            Q.emplace(parent, prep, 8);
            delete p;
            p = nullptr;
            Q.emplace(parent, prep, 9);
            return p;
        }
    }
    if(!check) {
        Q.emplace(parent, prep, 11);
        if(p->left == nullptr) {
            check = true;
            Q.emplace(parent, prep, 12);
            p->data = p->right->data;
            Q.emplace(parent, prep, 13);
            delete p->right;
            p->right = nullptr;
        }
    }
    if(!check) {
        Q.emplace(parent, prep, 16);
        Node *tmp = MaxNode(p->left);
        Q.emplace(parent, prep, 17);
        p->data = tmp->data;
        Q.emplace(parent, prep, 18);
        p->left = removeSBS(prep, prep->left, p->left, p->data);
    }
    Q.emplace(parent, prep, 20);
    p->depth = max(height(p->left), height(p->right)) + 1;
    Q.emplace(parent, prep, 21);
    if(p->left && height(p->left->right) > height(p->left->left) && height(p->left->right) - height(p->right) == 1) {
        Q.emplace(parent, prep, 22);
        p->left = leftrotate(p->left);
    }
    Q.emplace(parent, prep, 23);
    if(p->left && height(p->left->left) - height(p->right) == 1) {
        Q.emplace(parent, prep, 24);
        p = rightrotate(p);
    }
    Q.emplace(parent, prep, 25);
    if(p->right && height(p->right->left) > height(p->right->right) && height(p->right->left) - height(p->left) == 1) {
        Q.emplace(parent, prep, 26);
        p->right = rightrotate(p->right);
    }
    Q.emplace(parent, prep, 27);
    if(p->right && height(p->right->right) - height(p->left) == 1) {
        Q.emplace(parent, prep, 28);
        p = leftrotate(p);
    }
    Q.emplace(parent, prep, 29);
    return p;
}

void AVLTree::clear(Node *&p) {
    if(p == nullptr)
        return;
    clear(p->left);
    clear(p->right);
    delete p;
    p = nullptr;
}

void AVLTree::ResetFindSelected() {
    stack<Node*> SS;
    queue<Node*> Q;
    Q.emplace(root);
    while(!Q.empty()) {
        Node *p = Q.front();
        Q.pop();
        if(p == nullptr)
            continue;
        p->findselected = false;
        Q.emplace(p->left);
        Q.emplace(p->right);
    }
}

void AVLTree::UpdateDepth(Node *p) {
    if(p == nullptr)
        return;
    UpdateDepth(p->left);
    UpdateDepth(p->right);
    p->depth = max(height(p->left), height(p->right)) + 1;
}

void AVLTree::UpdatePosition(Node *p, int u, int v) {
    if(p == nullptr)
        return;
    p->newx = u;
    p->newy = v;
    if(p->x == -1) {
        p->x = p->newx;
        p->y = p->newy;
    }
    if(p->depth > 1) {
        int d = 35 * (1 << (p->depth - 2));
        UpdatePosition(p->left, u - d, v + 100);
        UpdatePosition(p->right, u + d, v + 100);
    }
}

void AVLTree::UpdatePosition2(Node *p, int u, int v) {
    if(p == nullptr)
        return;
    p->newx += u;
    p->x += u;
    p->newy += v;
    p->y += v;
    UpdatePosition2(p->left, u, v);
    UpdatePosition2(p->right, u, v);
}

void AVLTree::insert(int x) {
    PushHistory();
    root = insert(root, x);
    UpdatePosition(root, rootx, rooty);
}

void AVLTree::insertSBS(int x) {
    PushHistory();
    Node *p = nullptr;
    CopyData(p, root);
    p = insertSBS(nullptr, root, p, x);
    clear(p);
    Q.emplace(nullptr, nullptr, -1);
}

bool AVLTree::insertStepByStep() {
    Node *parent = get<0>(Q.front());
    Node *p = get<1>(Q.front());
    query = get<2>(Q.front());
    Q.pop();
    if(prep)
        prep->findselected = false;
    switch(query) {
        case 2:
            p = new Node(InsertNumber);
            if(parent == nullptr)
                root = p;
            else if(parent->data > p->data)
                parent->left = p;
            else
                parent->right = p;
            break;
        case 3:
            p = prep;
            break;
        case 9:
            p->left = leftrotate(p->left);
            break;
        case 11:
            p = rightrotate(p);
            if(parent == nullptr)
                root = p;
            else if(parent->data > p->data)
                parent->left = p;
            else
                parent->right = p;
            break;
        case 13:
            p = prep;
            break;
        case 17:
            p->right = rightrotate(p->right);
            break;
        case 19:
            p = leftrotate(p);
            if(parent == nullptr)
                root = p;
            else if(parent->data > p->data)
                parent->left = p;
            else
                parent->right = p;
            break;
        case 21:
            p = prep;
            break;
        default:
            break;
    }
    if(p != nullptr)
        p->findselected = true;
    prep = p;
    UpdateDepth(root);
    UpdatePosition(root, rootx, rooty);
    if(Q.empty())
        return false;
    return true;
}

void AVLTree::remove(int x) {
    PushHistory();
    root = remove(root, x);
    if(root)
        UpdatePosition(root, rootx, rooty);
}

void AVLTree::removeSBS(int x){
    PushHistory();
    Node *p = nullptr;
    CopyData(p, root);
    p = removeSBS(nullptr, root, p, x);
    clear(p);
    Q.emplace(nullptr, nullptr, -1);
}

bool AVLTree::removeStepByStep() {
    Node *parent = get<0>(Q.front());
    Node *p = get<1>(Q.front());
    query = get<2>(Q.front());
    Q.pop();
    if(prep)
        prep->findselected = false;
    switch(query) {
        case 8:
            if(parent == nullptr)
                root = nullptr;
            else if(parent->data >= p->data)
                parent->left = nullptr;
            else
                parent->right = nullptr;
            delete p;
            p = nullptr;
            break;
        case 9:
            p = prep;
            break;
        case 12:
            p->data = p->right->data;
            break;
        case 13:
            delete p->right;
            p->right = nullptr;
            break;
        case 17:
            p->data = MaxNode(p->left)->data;
            break;
        case 22:
            p->left = leftrotate(p->left);
            break;
        case 24:
            p = rightrotate(p);
            if(parent == nullptr)
                root = p;
            else if(parent->data > p->data)
                parent->left = p;
            else
                parent->right = p;
            break;
        case 25:
            p = prep;
            break;
        case 26:
            p->right = rightrotate(p->right);
            break;
        case 27:
            p = prep;
            break;
        case 28:
            p = leftrotate(p);
            if(parent == nullptr)
                root = p;
            else if(parent->data > p->data)
                parent->left = p;
            else
                parent->right = p;
            break;
        case 29:
            p = prep;
            break;
        default:
            break;
    }
    if(p != nullptr)
        p->findselected = true;
    prep = p;
    UpdateDepth(root);
    UpdatePosition(root, rootx, rooty);
    if(Q.empty())
        return false;
    return true;
}

bool AVLTree::find(int x) {
    bool res = false;
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
        res |= p->findselected;
        Q.emplace(p->left);
        Q.emplace(p->right);
    }
    return res;
}

void AVLTree::findSBS(int x) {
    Q.emplace(nullptr, root, 1);
    Node* p = root;
    Q.emplace(nullptr, p, 2);
    while(p != nullptr) {
        Q.emplace(nullptr, p, 3);
        if(p->data == x) {
            Q.emplace(nullptr, p, 4);
            Q.emplace(nullptr, nullptr, -1);
            return;
        }
        bool check = false;
        Q.emplace(nullptr, p, 5);
        if(p->data > x) {
            check = true;
            Q.emplace(nullptr, p, 6);
            p = p->left;
        }
        if(!check) {
            Q.emplace(nullptr, p, 8);
            p = p->right;
        }
        Q.emplace(nullptr, p, 2);
    }
    Q.emplace(nullptr, p, 10);
    Q.emplace(nullptr, nullptr, -1);
    return;
}

bool AVLTree::findStepByStep() {
    Node *parent = get<0>(Q.front());
    Node *p = get<1>(Q.front());
    query = get<2>(Q.front());
    Q.pop();
    if(prep)
        prep->findselected = false;
    switch(query) {
        case 4:
            CheckFind = 2;
            break;
        case 10:
            CheckFind = 1;
            break;
        default:
            break;
    }
    if(p != nullptr)
        p->findselected = true;
    prep = p;
    if(Q.empty())
        return false;
    return true;
}

void AVLTree::Draw(Node *p) {
    double dx = abs(p->x - p->newx);
    double dy = abs(p->y - p->newy);
    double d = sqrt(dx * dx + dy * dy);
    if(p->x < p->newx) {
        p->x += Speed / 10 * dx / d;
        if(p->x > p->newx)
            p->x = p->newx;
    }
    else if(p->x > p->newx) {
        p->x -= Speed / 10 * dx / d;
        if(p->x < p->newx)
            p->x = p->newx;
    }
    if(p->y < p->newy) {
        p->y += Speed / 10 * dy / d;
        if(p->y > p->newy)
            p->y = p->newy;
    }
    else if(p->y > p->newy) {
        p->y -= Speed / 10 * dy / d;
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
        clear(RedoHistory.top());
        RedoHistory.pop();
    }
}

void AVLTree::PopHistory() {
    if(History.empty())
        return;
    RedoHistory.emplace(root);
    root = History.top();
    UpdatePosition2(root, rootx - root->x, rooty - root->y);
    History.pop();
}

void AVLTree::PopRedoHistory() {
    if(RedoHistory.empty())
        return;
    History.emplace(root);
    root = RedoHistory.top();
    UpdatePosition2(root, rootx - root->x, rooty - root->y);
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

bool InsertAVLSBS() {
    if(Number.empty())
        return false;
    InsertNumber = 0;
    for(char c : Number)
        InsertNumber = 10 * InsertNumber + c - '0';
    S.insertSBS(InsertNumber);
    return true;
}

void RemoveAVL() {
    if(Number.empty())
        return;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    S.remove(x);
}

bool RemoveAVLSBS() {
    if(Number.empty())
        return false;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    S.removeSBS(x);
    return true;
}

void FindAVL() {
    if(Number.empty())
        return;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    CheckFind = S.find(x) + 1;
}

bool FindAVLSBS() {
    if(Number.empty())
        return false;
    int x = 0;
    for(char c : Number)
        x = 10 * x + c - '0';
    S.findSBS(x);
    return true;
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
    DrawText(Number.c_str(), x, y, fs, BLACK);
}

int MeasureNumber(int Cursor) {
    string c;
    for(int i = 0; i < Cursor; ++i)
        c += Number[i];
    return MeasureText(c.c_str(), 20);
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
    DrawNumber(130, 105, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 130 + MeasureNumber(CurrentCursor), 105, 20, BLACK);
}

void DrawDelete() {
    DrawNumber(130, 145, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 130 + MeasureNumber(CurrentCursor), 145, 20, BLACK);
}

void DrawFind() {
    DrawNumber(130, 185, 20);
    framecount = (framecount + 1) % 60;
    if(framecount < 30)
        DrawText("|", 130 + MeasureNumber(CurrentCursor), 185, 20, BLACK);
}

void DrawAddSBS() {
    DrawText("Press -> (right arrow) to go to the next step", 230, 10, 20, BLACK);
    DrawText("Press Enter to go to the result", 230, 35, 20, BLACK);
    for(int i = 0; i < 23; ++i) {
        if(query == i)
            DrawRectangle(10, 300 + 25 * i, MeasureText(ADDSTEPBYSTEP[i].c_str(), 20), 20, GRAY);
        DrawText(ADDSTEPBYSTEP[i].c_str(), 10, 300 + 25 * i, 20, BLACK);
    }
}

void DrawDeleteSBS() {
    DrawText("Press -> (right arrow) to go to the next step", 230, 10, 20, BLACK);
    DrawText("Press Enter to go to the result", 230, 35, 20, BLACK);
    if(query != -1)
        DrawRectangle(10 + MeasureText(DELETESTEPBYSTEP2[query].first.second.c_str(), 20), 300 + 25 * DELETESTEPBYSTEP2[query].first.first, MeasureText(DELETESTEPBYSTEP2[query].second.c_str(), 20), 20, GRAY);
    for(int i = 0; i < 24; ++i)
        DrawText(DELETESTEPBYSTEP[i].c_str(), 10, 300 + 25 * i, 20, BLACK);
}

void DrawFindSBS() {
    DrawText("Press -> (right arrow) to go to the next step", 230, 10, 20, BLACK);
    DrawText("Press Enter to go to the result", 230, 35, 20, BLACK);
    for(int i = 0; i < 12; ++i) {
        if(query == i)
            DrawRectangle(10, 300 + 25 * i, MeasureText(FINDSTEPBYSTEP[i].c_str(), 20), 20, GRAY);
        DrawText(FINDSTEPBYSTEP[i].c_str(), 10, 300 + 25 * i, 20, BLACK);
    }
}

void DisplayTree() {
    ClearBackground(BACKGROUND);

    DrawTree();

    DrawRectangle(9, 9, 212, 42, BORDER);
    DrawRectangle(10, 10, 210, 40, HOVERED);
    DrawText("AVL Tree", 115 - MeasureText("AVL Tree", 30) / 2, 15, 30, TEXT);

    DrawRectangle(1379, 9, 212, 42, BORDER);
    if(MouseButtonPressed(1380, 10, 1590, 50))
        DrawRectangle(1380, 10, 210, 40, BUTTON);
    else
        DrawRectangle(1380, 10, 210, 40, HOVERED);
    DrawText("BACK", 1485 - MeasureText("BACK", 30) / 2, 15, 30, TEXT);

    DrawRectangle(9, 59, 102, 32, BORDER);
    if(MouseButtonPressed(10, 60, 110, 90))
        DrawRectangle(10, 60, 100, 30, BUTTON);
    else
        DrawRectangle(10, 60, 100, 30, HOVERED);
    DrawText("Initialize", 60 - MeasureText("Initialize", 20) / 2, 65, 20, TEXT);

    DrawRectangle(9, 99, 102, 32, BORDER);
    if(MouseButtonPressed(10, 100, 110, 130))
        DrawRectangle(10, 100, 100, 30, BUTTON);
    else
        DrawRectangle(10, 100, 100, 30, HOVERED);
    DrawText("Add", 60 - MeasureText("Add", 20) / 2, 105, 20, TEXT);

    DrawRectangle(119, 99, 102, 32, BORDER);
    DrawRectangle(120, 100, 100, 30, WHITE);

    DrawRectangle(9, 139, 102, 32, BORDER);
    if(MouseButtonPressed(10, 140, 110, 170))
        DrawRectangle(10, 140, 100, 30, BUTTON);
    else
        DrawRectangle(10, 140, 100, 30, HOVERED);
    DrawText("Delete", 60 - MeasureText("Delete", 20) / 2, 145, 20, TEXT);

    DrawRectangle(119, 139, 102, 32, BORDER);
    DrawRectangle(120, 140, 100, 30, WHITE);

    DrawRectangle(9, 179, 102, 32, BORDER);
    if(MouseButtonPressed(10, 180, 110, 210))
        DrawRectangle(10, 180, 100, 30, BUTTON);
    else
        DrawRectangle(10, 180, 100, 30, HOVERED);
    DrawText("Find", 60 - MeasureText("Find", 20) / 2, 185, 20, TEXT);

    DrawRectangle(119, 179, 102, 32, BORDER);
    DrawRectangle(120, 180, 100, 30, WHITE);

    if(CheckFind > 0)
        DrawText(CheckFind == 2 ? "YES" : "NO", 230, 185, 20, BLACK);

    DrawRectangle(9, 219, 102, 32, BORDER);
    if(MouseButtonPressed(10, 220, 110, 250))
        DrawRectangle(10, 220, 100, 30, BUTTON);
    else
        DrawRectangle(10, 220, 100, 30, HOVERED);
    DrawText("Undo", 60 - MeasureText("Undo", 20) / 2, 225, 20, TEXT);

    DrawRectangle(119, 219, 102, 32, BORDER);
    if(MouseButtonPressed(120, 220, 220, 250))
        DrawRectangle(120, 220, 100, 30, BUTTON);
    else
        DrawRectangle(120, 220, 100, 30, HOVERED);
    DrawText("Redo", 170 - MeasureText("Redo", 20) / 2, 225, 20, TEXT);

    DrawRectangle(9, 259, 212, 32, BORDER);
    if(MouseButtonPressed(10, 260, 220, 290))
        DrawRectangle(10, 260, 210, 30, BUTTON);
    else
        DrawRectangle(10, 260, 210, 30, HOVERED);
    if(STEPBYSTEPBUTTON)
        DrawText("Step by step: ON", 115 - MeasureText("Step by step: ON", 20) / 2, 265, 20, TEXT);
    else
        DrawText("Step by step: OFF", 115 - MeasureText("Step by step: OFF", 20) / 2, 265, 20, TEXT);    

    DrawRectangle(1350, 840, 200, 10, HOVERED);
    DrawCircle(1349 + Speed, 845, 10, BORDER);

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
    else if(CurrentButton == ADDSBS)
        DrawAddSBS();
    else if(CurrentButton == DELETESBS)
        DrawDeleteSBS();
    else if(CurrentButton == FINDSBS)
        DrawFindSBS();
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

void BackToMainMenu() {
    if(!S.Q.empty())
        while(S.insertStepByStep());
    currentScreen = MAINMENU;
    Number = "";
    CurrentCursor = 0;
    STEPBYSTEPBUTTON = false;
    CheckFind = 0;
    rootx = 1000;
    rooty = 100;
    oldx = 0;
    oldy = 0;
    Pressed = false;
    InsertNumber = 0;
    query = -1;
    Speed = 10;
    Startx = -1;
    Starty = -1;
    S.clear(S.root);
    while(!S.History.empty()) {
        S.clear(S.History.top());
        S.History.pop();
    }
    while(!S.RedoHistory.empty()) {
        S.clear(S.RedoHistory.top());
        S.RedoHistory.pop();
    }
}

void TREE_INTERACT() {
    Pressed |= IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if(CurrentButton == NULLBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 100, 220, 130)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 140, 220, 170)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 140, 110, 170)) {
                Number = S.FindSelectedNode(S.root);
                S.ResetFindSelected();
                CheckFind = 0;
                if(STEPBYSTEPBUTTON) {
                    if(RemoveAVLSBS())
                        CurrentButton = DELETESBS;
                    else
                        CurrentButton = NULLBUTTON;
                }
                else {
                    RemoveAVL();
                    CurrentButton = NULLBUTTON;
                }
            }
            else if(MouseButtonPressed(120, 180, 220, 210)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 220, 110, 250))
                S.PopHistory();
            else if(MouseButtonPressed(120, 220, 220, 250))
                S.PopRedoHistory();
            else if(MouseButtonPressed(10, 260, 220, 290))
                STEPBYSTEPBUTTON ^= true;
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
            S.UpdateSelectedNode(S.root);
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
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
            else if(MouseButtonPressed(120, 100, 220, 130)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 140, 220, 170)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 180, 220, 210)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 220, 110, 250)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 220, 220, 250)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 260, 220, 290)) {
                STEPBYSTEPBUTTON ^= true;
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
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
            else if(MouseButtonPressed(120, 100, 220, 130)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 140, 220, 170)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 180, 220, 210)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 220, 110, 250)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 220, 220, 250)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 260, 220, 290)) {
                STEPBYSTEPBUTTON ^= true;
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
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
            else if(MouseButtonPressed(120, 100, 220, 130)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 140, 220, 170)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 180, 220, 210)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 220, 110, 250)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 220, 220, 250)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 260, 220, 290)) {
                STEPBYSTEPBUTTON ^= true;
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else
            UpdatePath();
    }
    else if(CurrentButton == ADDBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(10, 100, 110, 130)) {
                Startx = GetMousePosition().x;
                Starty = GetMousePosition().y;
                S.ResetFindSelected();
                CheckFind = 0;
                if(STEPBYSTEPBUTTON) {
                    if(InsertAVLSBS())
                        CurrentButton = ADDSBS;
                    else
                        CurrentButton = NULLBUTTON;
                } 
                else {
                    InsertAVL();
                    CurrentButton = NULLBUTTON;
                }
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 140, 220, 170)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(S.UpdateSelectedNode(S.root))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(120, 180, 220, 210)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 220, 110, 250)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 220, 220, 250)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 260, 220, 290))
                STEPBYSTEPBUTTON ^= true;
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
        }
        else
            UpdateNumber(true);
    }
    else if(CurrentButton == DELETEBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(10, 140, 110, 170)) {
                S.ResetFindSelected();
                CheckFind = 0;
                if(STEPBYSTEPBUTTON) {
                    if(RemoveAVLSBS())
                        CurrentButton = DELETESBS;
                    else
                        CurrentButton = NULLBUTTON;
                }
                else {
                    RemoveAVL();
                    CurrentButton = NULLBUTTON;
                }
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 100, 220, 130)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(S.UpdateSelectedNode(S.root))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(120, 180, 220, 210)) {
                CurrentButton = FINDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(10, 220, 110, 250)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 220, 220, 250)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 260, 220, 290))
                STEPBYSTEPBUTTON ^= true;
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
        }
        else
            UpdateNumber(true);
    }
    else if(CurrentButton == FINDBUTTON) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(10, 180, 110, 210)) {
                S.ResetFindSelected();
                CheckFind = 0;
                if(STEPBYSTEPBUTTON) {
                    if(FindAVLSBS())
                        CurrentButton = FINDSBS;
                    else
                        CurrentButton = NULLBUTTON;
                }
                else {
                    FindAVL();
                    CurrentButton = NULLBUTTON;
                }
            }
            else if(MouseButtonPressed(10, 60, 110, 90))
                CurrentButton = INITIALIZEBUTTON;
            else if(MouseButtonPressed(120, 100, 220, 130)) {
                CurrentButton = ADDBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(MouseButtonPressed(120, 140, 220, 170)) {
                CurrentButton = DELETEBUTTON;
                Number.clear();
                CurrentCursor = 0;
            }
            else if(S.UpdateSelectedNode(S.root))
                CurrentButton = NULLBUTTON;
            else if(MouseButtonPressed(10, 220, 110, 250)) {
                S.PopHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(120, 220, 220, 250)) {
                S.PopRedoHistory();
                CurrentButton = NULLBUTTON;
            }
            else if(MouseButtonPressed(10, 260, 220, 290))
                STEPBYSTEPBUTTON ^= true;
            else if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
        }
        else
            UpdateNumber(true);
    }
    else if(CurrentButton == ADDSBS) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else if(IsKeyPressed(KEY_RIGHT)) {
            if(!S.insertStepByStep())
                CurrentButton = NULLBUTTON;
        }
        else if(IsKeyPressed(KEY_ENTER)) {
            while(S.insertStepByStep());
            CurrentButton = NULLBUTTON;
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
        }
    }
    else if(CurrentButton == DELETESBS) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else if(IsKeyPressed(KEY_RIGHT)) {
            if(!S.removeStepByStep())
                CurrentButton = NULLBUTTON;
        }
        else if(IsKeyPressed(KEY_ENTER)) {
            while(S.removeStepByStep());
            CurrentButton = NULLBUTTON;
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
        }
    }
    else if(CurrentButton == FINDSBS) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Startx = GetMousePosition().x;
            Starty = GetMousePosition().y;
            if(MouseButtonPressed(1380, 10, 1590, 50))
                BackToMainMenu();
        }
        else if(IsKeyPressed(KEY_RIGHT)) {
            if(!S.findStepByStep())
                CurrentButton = NULLBUTTON;
        }
        else if(IsKeyPressed(KEY_ENTER)) {
            while(S.findStepByStep());
            CurrentButton = NULLBUTTON;
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if(Pressed) {
                if(Startx >= 1340 && Startx < 1560 && Starty >= 835 && Starty < 855) {
                    Speed = GetMousePosition().x - 1349;
                    if(Speed < 1)
                        Speed = 1;
                    else if(Speed > 200)
                        Speed = 200;
                }
                else {
                    int deltax = GetMousePosition().x - oldx;
                    int deltay = GetMousePosition().y - oldy;
                    rootx += deltax;
                    rooty += deltay;
                    S.UpdatePosition2(S.root, deltax, deltay);
                }
            }
        }
    }
    oldx = GetMousePosition().x;
    oldy = GetMousePosition().y;
}
