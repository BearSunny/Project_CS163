#include "mainmenu.h"
#include "LinkedListVisualizer.h"
#include "hashvisual.h"
#include "treevisual.h"
#include "graphvisual.h"

LinkedList* linkedList = nullptr;
LinkedListVisualizer* listVisualizer = nullptr;

bool MouseButtonPressed(float x, float y, float u, float v) {
    Vector2 p = GetMousePosition();
    if(p.x >= x && p.x < u && p.y >= y && p.y < v)
        return true;
    return false;
}

void Updatenumber() {
    if(IsKeyPressed(KEY_ZERO))
        Insertnumber(0);
    else if(IsKeyPressed(KEY_ONE))
        Insertnumber(1);
    else if(IsKeyPressed(KEY_TWO))
        Insertnumber(2);
    else if(IsKeyPressed(KEY_THREE))
        Insertnumber(3);
    else if(IsKeyPressed(KEY_FOUR))
        Insertnumber(4);
    else if(IsKeyPressed(KEY_FIVE))
        Insertnumber(5);
    else if(IsKeyPressed(KEY_SIX))
        Insertnumber(6);
    else if(IsKeyPressed(KEY_SEVEN))
        Insertnumber(7);
    else if(IsKeyPressed(KEY_EIGHT))
        Insertnumber(8);
    else if(IsKeyPressed(KEY_NINE))
        Insertnumber(9);
    else if(IsKeyPressed(KEY_BACKSPACE))
        Deletenumber();
}

void InitLinkedList() {
    if (linkedList == nullptr) {
        linkedList = new LinkedList();
        // Add some initial nodes for demonstration
        // FOR DEMONSTRATION ONLY
        // Feature to add manually and upload files is not done
        linkedList->add(10);
        linkedList->add(20);
        linkedList->add(30);
        linkedList->add(40);
        linkedList->add(50);
    }
    
    if (listVisualizer == nullptr) {
        listVisualizer = new LinkedListVisualizer(linkedList);
        listVisualizer->init();
    }
}

void CleanupLinkedList() {
    if (listVisualizer != nullptr) {
        delete listVisualizer;
        listVisualizer = nullptr;
    }
    
    if (linkedList != nullptr) {
        delete linkedList;
        linkedList = nullptr;
    }
}

void DisplayLinkedList() {
    DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    DrawText("Back to Main Menu", 20, 40, 20, BLACK);
    
    if (listVisualizer != nullptr) {
        listVisualizer->draw();
        listVisualizer->handleEvent();
    }
}

int main () {
    InitWindow(screenWidth, screenHeight, "DSA VISUALIZATION");
    GameScreen currentScreen = MAINMENU;
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        switch(currentScreen) {
            case MAINMENU:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 500, 400)) {
                        currentScreen = LINKEDLIST;
                        InitLinkedList();
                    }
                    else if(MouseButtonPressed(500, 0, 1000, 400))
                        currentScreen = HASH;
                    else if(MouseButtonPressed(0, 400, 500, 800))
                        currentScreen = TREE;
                    else if(MouseButtonPressed(500, 400, 1000, 800))
                        currentScreen = GRAPH;
                }
                break;
            case LINKEDLIST:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 100)) {
                        currentScreen = MAINMENU;
                        CleanupLinkedList();
                    }
                }
                break;
            case HASH:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 100))
                        currentScreen = MAINMENU;
                }
                break;
            case TREE:
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
                }
                else if(InsertButton || DeleteButton || FindButton)
                    Updatenumber();
                break;
            case GRAPH:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 100))
                        currentScreen = MAINMENU;
                }
                break;
            default:
                break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(currentScreen) {
            case MAINMENU:
                DisplayMainMenu();
                break;
            case LINKEDLIST:
                DisplayLinkedList();
                break;
            case HASH:
                DisplayHash();
                break;
            case TREE:
                DisplayTree();
                break;
            case GRAPH:
                DisplayGraph();
                break;
            default:
                break;
        }
        EndDrawing();
    }
    CleanupLinkedList();
    CloseWindow();
}
