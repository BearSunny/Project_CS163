#ifndef LINKEDLISTVISUAL_H
#define LINKEDLISTVISUAL_H

#include "declare.h"

void DisplayLinkedList() {
    DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, DARKBROWN);
}

#endif