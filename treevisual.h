#ifndef TREE_H
#define TREE_H

#include "declare.h"

void DisplayTree() {
    DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, DARKBROWN);    
}

#endif
