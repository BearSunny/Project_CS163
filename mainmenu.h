#ifndef MAINMENU_H
#define MAINMENU_H

#include "declare.h"

void DisplayMainMenu() {
    DrawRectangle(0, 0, 400, 400, LIGHTGRAY);
    DrawText("LINKED LIST", 200 - MeasureText("LINKED LIST", 40) / 2, 180, 40, DARKBROWN);

    DrawRectangle(400, 0, 400, 400, GRAY);
    DrawText("HASH", 600 - MeasureText("HASH", 40) / 2, 180, 40, BROWN);

    DrawRectangle(0, 400, 400, 400, DARKGRAY);
    DrawText("TREE", 200 - MeasureText("TREE", 40) / 2, 580, 40, BEIGE);

    DrawRectangle(400, 400, 400, 400, YELLOW);
    DrawText("GRAPH", 600 - MeasureText("GRAPH", 40) / 2, 580, 40, DARKPURPLE);
}

#endif