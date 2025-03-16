#ifndef MAINMENU_H
#define MAINMENU_H

#include "declare.h"

void DisplayMainMenu() {
    DrawRectangle(0, 0, 500, 400, LIGHTGRAY);
    DrawText("LINKED LIST", 250 - MeasureText("LINKED LIST", 40) / 2, 180, 40, DARKBROWN);

    DrawRectangle(500, 0, 500, 400, GRAY);
    DrawText("HASH", 750 - MeasureText("HASH", 40) / 2, 180, 40, BROWN);

    DrawRectangle(0, 400, 500, 400, DARKGRAY);
    DrawText("TREE", 250 - MeasureText("TREE", 40) / 2, 580, 40, BEIGE);

    DrawRectangle(500, 400, 500, 400, YELLOW);
    DrawText("GRAPH", 750 - MeasureText("GRAPH", 40) / 2, 580, 40, DARKPURPLE);
}

#endif
