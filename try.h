/*
#ifndef LINKEDLISTVISUALIZER_H
#define LINKEDLISTVISUALIZER_H

#include <raylib.h>
#include <string>
#include <vector>
#include "declare.h"
#include "LinkedList.h"
/*
enum VisualizerMode {
    MODE_NONE,
    MODE_DELETE,
    MODE_UPDATE,
    MODE_SEARCH,
    MODE_CREATE_MANUAL,
    MODE_CREATE_FILE
};

class LinkedListVisualizer {
private:
    LinkedList* list;
    bool isPaused;
    float animationSpeed;
    int inputNumber;
    std::string lastOperation;
    VisualizerMode currentMode;
    
    // For step-by-step visualization
    struct Operation {
        VisualizerMode mode;
        int value;
        int index;  // For update operations
    };
    std::vector<Operation> operationHistory;
    int historyIndex;
    
    // Helper methods
    void SaveOperation(VisualizerMode mode, int value, int index = -1);
    void ProcessNumberInput();

public:
    LinkedListVisualizer(LinkedList* list);
    
    // Mode setters
    void SetMode(VisualizerMode mode);
    VisualizerMode GetCurrentMode() const;
    
    // Operation methods
    void InsertNode();
    void DeleteNode();
    void UpdateNode();
    void SearchNode();
    void CreateFromFile();
    
    // History navigation
    void UndoOperation();
    void StepBackward();
    void StepForward();
    
    // Input handling
    void Update();
    void HandleInput();
    
    // Drawing methods
    void DrawList();
    void DisplayList();
};


enum VisualizerMode {
    MODE_NONE,
    MODE_INITIALIZE, 
    MODE_ADD,
    MODE_DELETE,
    MODE_UPDATE,
    MODE_SEARCH,
    MODE_CREATE_MANUAL,
    MODE_CREATE_FILE
};

struct Operation {
    enum Type { ADD, DELETE, UPDATE, SEARCH } type;
    int nodeIndex;
    int oldValue;
    int newValue;
    Operation(Type t, int idx, int oldVal = 0, int newVal = 0);
};

class LinkedListVisualizer {
public:
    LinkedListVisualizer(LinkedList* list);
    void init();
    void draw();
    void handleEvent();
    void createManualList();
    void createLLFromValues(const std::vector<int>& values);
    bool createLLFromFile(const std::string& filePath);
    void drawManualCreationInterface();
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

// Implementation

LinkedListVisualizer::LinkedListVisualizer(LinkedList* list)
    : list(list), 
      isPaused(false), 
      animationSpeed(1.0f), 
      inputNumber(0),
      lastOperation(""),
      currentMode(MODE_NONE),
      historyIndex(-1) {}

void LinkedListVisualizer::SetMode(VisualizerMode mode) {
    currentMode = mode;
    inputNumber = 0;
}

VisualizerMode LinkedListVisualizer::GetCurrentMode() const {
    return currentMode;
}

void LinkedListVisualizer::ProcessNumberInput() {
    // Convert char array to int
    inputNumber = atoi(number);
}

void LinkedListVisualizer::SaveOperation(VisualizerMode mode, int value, int index) {
    // Remove any future history if we're not at the end
    if (historyIndex < static_cast<int>(operationHistory.size()) - 1) {
        operationHistory.resize(historyIndex + 1);
    }
    
    // Add new operation to history
    Operation op = {mode, value, index};
    operationHistory.push_back(op);
    historyIndex = operationHistory.size() - 1;
    
    // Update last operation description
    switch (mode) {
        case MODE_CREATE_MANUAL:
            lastOperation = "Added node with value " + std::to_string(value);
            break;
        case MODE_DELETE:
            lastOperation = "Deleted node with value " + std::to_string(value);
            break;
        case MODE_UPDATE:
            lastOperation = "Updated node at index " + std::to_string(index) + 
                           " to value " + std::to_string(value);
            break;
        case MODE_SEARCH:
            lastOperation = "Searched for value " + std::to_string(value);
            break;
        case MODE_CREATE_FILE:
            lastOperation = "Created list from file";
            break;
        default:
            lastOperation = "";
            break;
    }
}

void LinkedListVisualizer::InsertNode() {
    ProcessNumberInput();
    if (inputNumber == 0) return;
    
    list->add(inputNumber);
    SaveOperation(MODE_CREATE_MANUAL, inputNumber);
    
    // Reset input
    length = 0;
    number[0] = '\0';
    inputNumber = 0;
}

// NEED MORE CONSIDERATION
void LinkedListVisualizer::DeleteNode() {
    ProcessNumberInput();
    if (inputNumber == 0) return;
    
    bool success = list->deleteAt(inputNumber);
    if (success) {
        SaveOperation(MODE_DELETE, inputNumber);
    } else {
        lastOperation = "Value " + std::to_string(inputNumber) + " not found";
    }
    
    // Reset input
    length = 0;
    number[0] = '\0';
    inputNumber = 0;
}

void LinkedListVisualizer::UpdateNode() {
    ProcessNumberInput();
    if (inputNumber == 0) return;
    
    // For update, we need two inputs: index and value
    // This is a simplified version - you might want to implement a more sophisticated input system
    static bool isGettingIndex = true;
    static int updateIndex = -1;
    
    if (isGettingIndex) {
        updateIndex = inputNumber;
        isGettingIndex = false;
        lastOperation = "Enter new value";
    } else {
        int updateValue = inputNumber;
        list->update(updateIndex, updateValue);
        SaveOperation(MODE_UPDATE, updateValue, updateIndex);
        //lastOperation = "Invalid index " + std::to_string(updateIndex);
        isGettingIndex = true;
        updateIndex = -1;
    }
    
    // Reset input
    length = 0;
    number[0] = '\0';
    inputNumber = 0;
}

void LinkedListVisualizer::SearchNode() {
    ProcessNumberInput();
    if (inputNumber == 0) return;
    
    bool found = list->search(inputNumber);
    SaveOperation(MODE_SEARCH, inputNumber);
    if (found) {
        lastOperation = "Found value " + std::to_string(inputNumber);
    } else {
        lastOperation = "Value " + std::to_string(inputNumber) + " not found";
    }
    
    // Reset input
    length = 0;
    number[0] = '\0';
    inputNumber = 0;
}

void LinkedListVisualizer::CreateFromFile() {
    // Implementation would read integers from a file and add them to the list
    list->clear();
    
    // Open file dialog would be implemented here
    
    // For now, just add some sample values
    list->add(10);
    list->add(20);
    list->add(30);
    SaveOperation(MODE_CREATE_FILE, 0);
    
    lastOperation = "Created list from file";
}

void LinkedListVisualizer::UndoOperation() {
    if (historyIndex >= 0) {
        // Get the operation to undo
        VisualizerMode mode = operationHistory[historyIndex].mode;
        int value = operationHistory[historyIndex].value;
        
        // Perform the opposite operation
        switch (mode) {
            case MODE_CREATE_MANUAL:
                list->deleteAt(value);
                lastOperation = "Undid add operation";
                break;
            case MODE_DELETE:
                list->add(value);
                lastOperation = "Undid delete operation";
                break;
            case MODE_UPDATE:
                // For update, we would need to store the previous value
                lastOperation = "Cannot undo update operation";
                break;
            default:
                lastOperation = "Cannot undo this operation";
                break;
        }
        
        // Remove the operation from history
        operationHistory.pop_back();
        historyIndex--;
    } else {
        lastOperation = "Nothing to undo";
    }
}

void LinkedListVisualizer::StepBackward() {
    if (historyIndex > 0) {
        historyIndex--;
        // Recreate the list up to historyIndex
        list->clear();
        for (int i = 0; i <= historyIndex; i++) {
            if (operationHistory[i].mode == MODE_CREATE_MANUAL) {
                list->add(operationHistory[i].value);
            }
            // Other operations would be handled here
        }
        lastOperation = "Stepped backward";
    } else {
        lastOperation = "At beginning of history";
    }
}

void LinkedListVisualizer::StepForward() {
    if (historyIndex < static_cast<int>(operationHistory.size()) - 1) {
        historyIndex++;
        // Apply the next operation
        VisualizerMode mode = operationHistory[historyIndex].mode;
        int value = operationHistory[historyIndex].value;
        
        switch (mode) {
            case MODE_CREATE_MANUAL:
                list->add(value);
                break;
            case MODE_DELETE:
                list->deleteAt(value);
                break;
            // Other operations would be handled here
        }
        lastOperation = "Stepped forward";
    } else {
        lastOperation = "At end of history";
    }
}

void LinkedListVisualizer::HandleInput() {
    // Handle number input
    for (int i = 0; i <= 9; i++) {
        if (IsKeyPressed(KEY_ZERO + i)) {
            Insertnumber(i);
        }
    }
    
    // Handle backspace
    if (IsKeyPressed(KEY_BACKSPACE)) {
        Deletenumber();
    }
    
    // Handle enter for confirming operations
    if (IsKeyPressed(KEY_ENTER)) {
        switch (currentMode) {
            case MODE_CREATE_MANUAL:
                InsertNode();
                break;
            case MODE_DELETE:
                DeleteNode();
                break;
            case MODE_UPDATE:
                UpdateNode();
                break;
            case MODE_SEARCH:
                SearchNode();
                break;
            default:
                break;
        }
    }
    
    // Handle mode selection
    if (IsKeyPressed(KEY_ONE)) SetMode(MODE_CREATE_MANUAL);
    if (IsKeyPressed(KEY_TWO)) SetMode(MODE_DELETE);
    if (IsKeyPressed(KEY_THREE)) SetMode(MODE_UPDATE);
    if (IsKeyPressed(KEY_FOUR)) SetMode(MODE_SEARCH);
    if (IsKeyPressed(KEY_SIX)) SetMode(MODE_CREATE_FILE);
    
    // Handle navigation
    if (IsKeyPressed(KEY_Z)) UndoOperation();
    if (IsKeyPressed(KEY_LEFT)) StepBackward();
    if (IsKeyPressed(KEY_RIGHT)) StepForward();
    
    // Handle buttons
    if (InsertButton) {
        InsertButton = false;
        InsertNode();
    }
    
    if (DeleteButton) {
        DeleteButton = false;
        DeleteNode();
    }
    
    if (FindButton) {
        FindButton = false;
        SearchNode();
    }
}

void LinkedListVisualizer::Update() {
    HandleInput();
    framecount++;
}

void LinkedListVisualizer::DrawList() {
    float startX = 100.f;
    float startY = 400.f;
    float offsetX = 100.f;
    Node* current = list->getHead();
    int index = 0;
    
    while (current) {
        Color nodeColor = WHITE;
        Color textColor = BLACK;
        
        // Highlight nodes based on current mode and search results
        if (currentMode == MODE_SEARCH) {
            ProcessNumberInput();
            if (current->val == inputNumber) {
                nodeColor = GREEN;
                textColor = WHITE;
            }
        }
        
        DrawCircle(startX + index * offsetX, startY, 30, nodeColor);
        DrawText(TextFormat("%d", current->val), startX + index * offsetX - 10, startY - 10, 20, textColor);
        DrawText(TextFormat("%d", index), startX + index * offsetX - 5, startY + 30, 16, DARKGRAY);
        
        if (current->next) {
            DrawLine(startX + index * offsetX + 30, startY, 
                     startX + (index + 1) * offsetX - 30, startY, BLACK);
            DrawTriangle(
                Vector2{startX + (index + 1) * offsetX - 35, startY - 5},
                Vector2{startX + (index + 1) * offsetX - 35, startY + 5},
                Vector2{startX + (index + 1) * offsetX - 25, startY},
                BLACK
            );
        }
        
        current = current->next;
        index++;
    }
}

void LinkedListVisualizer::DisplayList() {
    ClearBackground(RAYWHITE);
    
    // Draw header
    DrawRectangle(0, 0, screenWidth, 50, BLUE);
    DrawText("LINKED LIST VISUALIZER", 20, 10, 30, YELLOW);
    
    // Draw mode indicator
    const char* modeText = "";
    switch (currentMode) {
        case MODE_CREATE_MANUAL: modeText = "Add Mode"; break;
        case MODE_DELETE: modeText = "Delete Mode"; break;
        case MODE_UPDATE: modeText = "Update Mode"; break;
        case MODE_SEARCH: modeText = "Search Mode"; break;
        case MODE_CREATE_FILE: modeText = "Create File Mode"; break;
        default: modeText = "No Mode Selected"; break;
    }
    DrawText(modeText, 400, 10, 20, WHITE);
    
    // Draw buttons
    DrawRectangle(50, 60, 100, 30, LIGHTGRAY);
    DrawText("Insert", 75, 65, 20, BLACK);
    
    DrawRectangle(200, 60, 100, 30, LIGHTGRAY);
    DrawText("Delete", 225, 65, 20, BLACK);
    
    DrawRectangle(350, 60, 100, 30, LIGHTGRAY);
    DrawText("Find", 385, 65, 20, BLACK);
    
    DrawRectangle(500, 60, 100, 30, LIGHTGRAY);
    DrawText("Undo", 525, 65, 20, BLACK);
    
    DrawRectangle(650, 60, 100, 30, LIGHTGRAY);
    DrawText("< Back", 670, 65, 20, BLACK);
    
    DrawRectangle(800, 60, 100, 30, LIGHTGRAY);
    DrawText("Next >", 820, 65, 20, BLACK);
    
    // Draw input area
    DrawRectangle(50, 100, 400, 40, WHITE);
    DrawRectangleLines(50, 100, 400, 40, BLACK);
    DrawText(number, 60, 110, 20, BLACK);
    
    // Show current operation message
    DrawText(lastOperation.c_str(), 50, 150, 20, DARKGRAY);
    
    // Draw instructions
    // WARNING CHECK THIS AGAIN
    DrawText("Mode Keys: 1-Add, 2-Delete, 3-Update, 4-Search, 5-Create Manual, 6-Create File", 
             50, 200, 16, DARKGRAY);
    DrawText("Navigation: Z-Undo, Left-Back, Right-Next", 50, 220, 16, DARKGRAY);
    DrawText("Actions: Enter-Confirm, Backspace-Delete Number", 50, 240, 16, DARKGRAY);

    DrawList();
}
#endif LINKEDLISTVISUALIZER_H
*/