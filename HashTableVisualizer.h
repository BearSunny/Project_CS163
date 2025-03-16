#pragma once

#include "HashTable.h";
#include "declare.h"

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
    int key;
    int oldValue;
    int newValue;
    Operation(Type t, int k, int oldVal = 0, int newVal = 0);
};

void InitHashTable();
void CleanupHashTable();
void DisplayHashTable();

class HashTableVisualizer {
public:
    HashTableVisualizer(HashTable* table);
    void init();
    void draw();
    void handleEvent();
    bool createHashTableFromFile(const std::string& filePath);
    void drawFileUploadInterface();

private:
    HashTable* table;
    VisualizerMode mode;
    std::string inputString;
    int selectedKey;
    std::vector<int> manualInputKeys;
    bool showFileDialog;
    char filePath[256];
    bool fileError;
    std::string fileErrorMessage;

    bool isPaused;
    float animationSpeed;
    float animationProgress;
    std::deque<Operation> operationHistory;
    std::deque<Operation> undoHistory;
    int currentStep;
    std::string lastOperation;

    void drawAnimationControls();
    void drawOperationInfo();
    void drawHashTable(float startX, float startY);
    void drawBucket(float posX, float posY, int key, int value);
    void drawConnection(float startX, float startY, float endX, float endY);
    void drawInputBox();
    void drawHelpText();

    void updateAnimation();
    void stepForward();
    void stepBackward();
    void applyOperation(const Operation& op);
    void undoOperation(const Operation& op);
    void applyAnimationEffects(float posX, float posY, int key, int value);

    // UI helper functions
    bool DrawButton(float x, float y, float width, float height, const char* text);
    float GuiSlider(Rectangle bounds, const char* textLeft, const char* textRight,
                   float value, float minValue, float maxValue);
};
