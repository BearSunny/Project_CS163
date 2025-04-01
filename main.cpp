#include "source/declare.cpp"
#include "source/mainmenu.cpp"
#include "source/LinkedListVisualizer0.cpp"
#include "source/LinkedList0.cpp"
#include "source/treevisual.cpp"
#include "source/Graph.cpp"
#include "source/GraphButton.cpp"
#include "header/hashvisual.h"
#include "header/HashTableVisual.h"
#include "source/HashTableVisual.cpp"

int main () {
    InitWindow(screenWidth, screenHeight, "DSA VISUALIZATION");
    initResource();
    SetTargetFPS(60);
    GraphVisualize graph = GraphVisualize(FONT);
    TittleButton title = TittleButton({465, 34, 350, 40}, "Graph", -1, BLACK, 20);
    HashTablePage hashPage;

    while(!WindowShouldClose()) {
        switch(currentScreen) {
            case MAINMENU:
                MAINMENU_INTERACT();
                break;
            case LINKEDLIST:
                LINKEDLIST_INTERACT();
                break;
            case HASH:
                if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 100))
                        currentScreen = MAINMENU;
                }
                hashPage.handleInput();
                break;
            case TREE:
                TREE_INTERACT();
                break;
            case GRAPH:
                graph.handle();
                /* if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if(MouseButtonPressed(0, 0, 400, 100))
                        currentScreen = MAINMENU;
                } */
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
                hashPage.draw();
                break;
            case TREE:
                DisplayTree();
                break;
            case GRAPH:
                ClearBackground(THEME.BACKGROUND);
                graph.draw();
                title.draw();
                break;
            default:
                break;
        }
        EndDrawing();
    }
    CleanupLinkedList();
    CloseWindow();
}
