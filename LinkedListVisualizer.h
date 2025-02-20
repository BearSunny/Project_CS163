#ifndef LINKEDLISTVISUALIZER_H
#define LINKEDLISTVISUALIZER_H

#include "DummyLinkedList.h"
#include <string>
#include <sstream>
using namespace std;

enum VisualizerMode {
    MODE_NONE,
    MODE_INITIALIZE, 
    MODE_ADD,
    MODE_DELETE,
    MODE_UPDATE,
    MODE_SEARCH
};

class LinkedListVisualizer {
public:
    LinkedListVisualizer(DummyLinkedList* list);
    void draw();
    void handleEvent();

private:
    DummyLinkedList* list;
    VisualizerMode mode;
    string inputString;
    int nodeIndex;
};

#endif // LINKEDLISTVISUALIZER_H