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
    MODE_CREATE_FILE,
    MODE_ADD_HEAD
};

// Add new enum for animation states
enum AnimationState {
    WAITING,
    TRAVERSING,
    FINAL_ANIMATION
};

struct Operation {
    enum Type { ADD, ADD_HEAD, DELETE, UPDATE, SEARCH } type;
    int nodeIndex;
    int oldValue;
    int newValue;
    Operation(Type t, int idx, int oldVal = 0, int newVal = 0);
    std::string toString();
};

void InitLinkedList();
void CleanupLinkedList();
void DisplayLinkedList();

void LINKEDLIST_INTERACT();

class LinkedListVisualizer {
public:
    LinkedListVisualizer(LinkedList* list);
    void init();
    void draw();
    void handleEvent();
    bool createLLFromFile(const std::string& filePath);
    void drawFileUploadInterface();
    void drawInitInterface();

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
    float arrowProgress;
    bool isPaused;
    float animationSpeed;
    float animationProgress;
    deque<Operation> operationHistory;
    deque<Operation> undoHistory;
    int currentStep;
    string lastOperation;
    std::vector<std::pair<float, float>> connectionAnimations;
    bool showPseudocode;
    int currentPseudocodeLine;
    float pseudocodeProgress;
    bool stepByStepMode;
    AnimationState animState;
    int traversalIndex;
    float nodeHighlightProgress;
    int pendingAddValue;
    bool shouldAddNode;
    int pendingTraversalCount;
    int pendingUpdateValue;
    bool updateDone;
    bool searchDone;
    int pendingTargetIndex;
    int pendingDeleteValue;
    bool deleteDone;

    void drawAnimationControls();
    void drawOperationInfo();
    void drawLinkedList(float startX, float startY, float offsetX);
    void drawNode(float posX, float posY, Node* node, int index);
    void drawConnection(float startX, float startY, float offsetX, int connectionIndex);
    void drawInputBox();
    void drawHelpText();
    void drawPseudocodeBox();

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
    void LINKEDLIST_INTERACT();
};

#endif // LINKEDLISTVISUALIZER_H
