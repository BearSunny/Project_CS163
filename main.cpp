#include "source/declare.cpp"
#include "source/mainmenu.cpp"

int main () {
    InitWindow(screenWidth, screenHeight, "DSA VISUALIZATION");
    initResource();
    SetTargetFPS(60);
    GraphVisualize graph = GraphVisualize(FONT);
    TittleButton title = TittleButton({465, 34, 350, 40}, "Graph", -1, BLACK, 20);
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
