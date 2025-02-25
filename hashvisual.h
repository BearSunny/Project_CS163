#ifndef HASH_H
#define HASH_H

#include "declare.h"

void DisplayHash() {
    DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, DARKBROWN);
}

#endif