#ifndef LINKEDLISTVISUALIZER_H
#define LINKEDLISTVISUALIZER_H

#include "LinkedList.h"
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <deque>
#include <fstream>
#include "declare.h"
using namespace std;

enum VisualizerMode {
    MODE_NONE,
    MODE_INITIALIZE, 
    MODE_ADD,
    MODE_DELETE,
    MODE_UPDATE,
    MODE_SEARCH,
    MODE_CREATE_FILE
};

struct Operation {
    enum Type { ADD, DELETE, UPDATE, SEARCH } type;
    int nodeIndex;
    int oldValue;
    int newValue;
    Operation(Type t, int idx, int oldVal = 0, int newVal = 0);
};

void InitLinkedList();
void CleanupLinkedList();
void DisplayLinkedList();

class LinkedListVisualizer {
public:
    LinkedListVisualizer(LinkedList* list);
    void init();
    void draw();
    void handleEvent();
    bool createLLFromFile(const std::string& filePath);
    void drawFileUploadInterface();

private:
    LinkedList* list;
    VisualizerMode mode;
    string inputString;
    int nodeIndex;
    int selectedNodeIndex;
    vector<int> manualInputValues;
    bool showFileDialog;
    char filePath[256];
    bool fileError;
    string fileErrorMessage;

    bool isPaused;
    float animationSpeed;
    float animationProgress;
    deque<Operation> operationHistory;
    deque<Operation> undoHistory;
    int currentStep;
    string lastOperation;

    void drawAnimationControls();
    void drawOperationInfo();
    void drawLinkedList(float startX, float startY, float offsetX);
    void drawNode(float posX, float posY, Node* node, int index);
    void drawConnection(float startX, float startY, float offsetX);
    void drawInputBox();
    void drawHelpText();

    void updateAnimation();
    void stepForward();
    void stepBackward();
    void applyOperation(const Operation& op);
    void undoOperation(const Operation& op);
    void applyAnimationEffects(float posX, float posY, Node* node, int index);

    // UI helper functions
    bool DrawButton(float x, float y, float width, float height, const char* text);
    float GuiSlider(Rectangle bounds, const char* textLeft, const char* textRight, 
                   float value, float minValue, float maxValue);
};

#endif // LINKEDLISTVISUALIZER_H