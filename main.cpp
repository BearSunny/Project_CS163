#include "mainmenu.h"
#include "linkedlistvisual.h"
#include "hashvisual.h"
#include "treevisual.h"
#include "graphvisual.h"

using namespace std;

bool MouseButtonPressed(float x, float y, float u, float v) {
    Vector2 p = GetMousePosition();
    if(p.x >= x && p.x < u && p.y >= y && p.y < v)
        return true;
    return false;
}

int main () {
    InitWindow(screenWidth, screenHeight, "DSA VISUALIZATION");
    GameScreen currentScreen = MAINMENU;
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        switch(currentScreen) {
            case MAINMENU:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 400))
                        currentScreen = LINKEDLIST;
                    else if(MouseButtonPressed(0, 400, 400, 800))
                        currentScreen = HASH;
                    else if(MouseButtonPressed(400, 0, 800, 400))
                        currentScreen = TREE;
                    else if(MouseButtonPressed(400, 400, 800, 800))
                        currentScreen = GRAPH;
                }
                break;
            case LINKEDLIST:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 100))
                        currentScreen = MAINMENU;
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
                    if(MouseButtonPressed(0, 0, 400, 100))
                        currentScreen = MAINMENU;
                }
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
}
