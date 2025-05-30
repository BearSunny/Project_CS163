#include "header/declare.h"
#include "header/mainmenu.h"
#include "header/LinkedListVisualizer.h"
#include "header/LinkedList.h"
#include "header/linkedlistvisual.h"
#include "header/treevisual.h"
#include "header/graphvisual.h"

#include "header/HashTableVisual.h"


int main () {
    InitWindow(screenWidth, screenHeight, "DSA VISUALIZATION");
    initResource();
    SetTargetFPS(60);
    GraphVisualize graph = GraphVisualize(FONT);
    TittleButton title = TittleButton({800, 34, 350, 40}, "Graph", -1, Color {255, 254, 206, 255}, 30);
    HashTablePage hashPage;
    MainPage mainmenu;

    while(!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        switch(currentScreen) {
            case MAINMENU:
                mainmenu.handleInput();
                mainmenu.update(deltaTime);
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
                hashPage.update(deltaTime);
                break;
            case TREE:
                TREE_INTERACT();
                break;
            case GRAPH:
                graph.handle(deltaTime);
                title.handle();
                if (title.getChoice() == 1)
                {
                    graph.clearGraph();
                    title.setChoice();
                }
                else if (title.getChoice() == 2)
                {
                    currentScreen = MAINMENU;
                    title.setChoice();
                }
                break;
            default:
                break;
        }

        BeginDrawing();
        ClearBackground((Color){241, 231, 231, 255});
        switch(currentScreen) {
            case MAINMENU:
                mainmenu.draw();
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
    CloseWindow();
}
