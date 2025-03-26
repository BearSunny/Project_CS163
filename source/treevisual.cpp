#include "header/treevisual.h"

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
    root = insert(root, x);
    UpdatePosition(root, 500, 100);
}

void AVLTree::remove(int x) {
    root = remove(root, x);
    UpdatePosition(root, 500, 100);
}

bool AVLTree::find(int x) {
    Node *p = root;
    while(p != nullptr) {
        if(p->data == x)
            return true;
        if(p->data > x)
            p = p->left;
        else
            p = p->right;
    }
    return false;
}

void AVLTree::Draw(Node *p) {
    if(p->x < p->newx)
        ++p->x;
    else if(p->x > p->newx)
        --p->x;
    if(p->y < p->newy)
        ++p->y;
    else if(p->y > p->newy)
        --p->y;
    if(p->left) {
        DrawLine(p->x, p->y, p->left->x, p->left->y, BLACK);
        Draw(p->left);        
    }
    if(p->right) {
        DrawLine(p->x, p->y, p->right->x, p->right->y, BLACK);
        Draw(p->right);
    }
    DrawCircle(p->x, p->y, 32, DARKGREEN);
    DrawCircle(p->x, p->y, 30, GREEN);
    DrawNumber(p->data, p->x, p->y, 20);
}

void InsertAVL() {
    if(length == 0)
        return;
    int x = 0;
    for(int i = 0; i < length; ++i)
        x = 10 * x + number[i] - '0';
    S.insert(x);
    FindQuerry = -1;
}

void RemoveAVL() {
    if(length == 0)
        return;
    int x = 0;
    for(int i = 0; i < length; ++i)
        x = 10 * x + number[i] - '0';
    S.remove(x);
    FindQuerry = -1;
}

void FindAVL() {
    if(length == 0)
        return;
    int x = 0;
    for(int i = 0; i < length; ++i)
        x = 10 * x + number[i] - '0';
    FindQuerry = S.find(x);
    FindNumber = x;
}

void DrawTree() {
    if(S.root)
        S.Draw(S.root);
}

void DisplayTree() {
    ClearBackground((Color){30, 30, 46, 255});

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

    if(FindQuerry != -1)
        DrawText(FindQuerry ? "YES" : "NO", 60, 60, 30, BLACK);
}

void TREE_INTERACT() {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if(MouseButtonPressed(200, 10, 300, 40)) {
            if(!InsertButton) {
                InsertButton = true;
                length = 0;
                number[0] = 0;
                framecount = 0;
            }
        }
        else
            InsertButton = false;
        if(MouseButtonPressed(305, 10, 405, 40))
            InsertAVL();
        if(MouseButtonPressed(410, 10, 510, 40)) {
            if(!DeleteButton) {
                DeleteButton = true;
                length = 0;
                number[0] = 0;
                framecount = 0;
            }
        }
        else
            DeleteButton = false;
        if(MouseButtonPressed(515, 10, 615, 40))
            RemoveAVL();
        if(MouseButtonPressed(620, 10, 720, 40)) {
            if(!FindButton) {
                FindButton = true;
                length = 0;
                number[0] = 0;
                framecount = 0;
            }
        }
        else
            FindButton = false;
        if(MouseButtonPressed(725, 10, 825, 40))
            FindAVL();
    }
    else if(InsertButton || DeleteButton || FindButton)
        Updatenumber();
}
