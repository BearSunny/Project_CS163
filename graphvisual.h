#ifndef GRAPH_H
#define GRAPH_H

#include "declare.h"

void DisplayGraph() {
    DrawRectangle(0, 0, 400, 100, LIGHTGRAY);
    DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, DARKBROWN);
}

#endif