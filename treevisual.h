#ifndef TREE_H
#define TREE_H

#include "declare.h"

void DisplayTree() {
    DrawRectangle(0, 0, 1000, 50, GREEN);
    DrawText("AVL TREE", 10, 10, 30, YELLOW);

    DrawRectangle(199, 9, 102, 32, BLACK);
    DrawRectangle(200, 10, 100, 30, WHITE);

    DrawRectangle(304, 9, 102, 32, BLACK);
    DrawRectangle(305, 10, 100, 30, LIGHTGRAY);
    DrawText("Insert", 355 - MeasureText("Insert", 20) / 2, 15, 20, BLACK);

    DrawRectangle(409, 9, 102, 32, BLACK);
    DrawRectangle(410, 10, 100, 30, WHITE);

    DrawRectangle(514, 9, 102, 32, BLACK);
    DrawRectangle(515, 10, 100, 30, LIGHTGRAY);
    DrawText("Delete", 565 - MeasureText("Delete", 20) / 2, 15, 20, BLACK);

    DrawRectangle(619, 9, 102, 32, BLACK);
    DrawRectangle(620, 10, 100, 30, WHITE);

    DrawRectangle(724, 9, 102, 32, BLACK);
    DrawRectangle(725, 10, 100, 30, LIGHTGRAY);
    DrawText("Find", 775 - MeasureText("Find", 20) / 2, 15, 20, BLACK);

    // DrawText("INSERT", 10, 15, 20, DARKBROWN);
    // DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    // DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, DARKBROWN); 
}

#endif
