#include "source/mainmenu.cpp"
#include "source/LinkedListVisualizer0.cpp"
#include "source/LinkedList0.cpp"
#include "source/treevisual.cpp"
#include "header/hashvisual.h"
#include "header/graphvisual.h"

int main () {
    InitWindow(screenWidth, screenHeight, "DSA VISUALIZATION");
    SetTargetFPS(60);
    while(!WindowShouldClose()) {
        switch(currentScreen) {
            case MAINMENU:
                MAINMENU_INTERACT();
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
                TREE_INTERACT();
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
