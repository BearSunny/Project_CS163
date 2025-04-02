#ifndef LINKEDLISTVISUALIZER0_H
#define LINKEDLISTVISUALIZER0_H

#include "header/LinkedListVisualizer.h"
#include <sstream>
#include <raylib.h>
#include <cmath>
#include <fstream>
#include <deque>


LinkedList* linkedList = nullptr;
LinkedListVisualizer* listVisualizer = nullptr;

void InitLinkedList() {
    if (linkedList == nullptr) {
        linkedList = new LinkedList();
    }
    
    if (listVisualizer == nullptr) {
        listVisualizer = new LinkedListVisualizer(linkedList);
        listVisualizer->init();
    }
}

void CleanupLinkedList() {
    if (listVisualizer != nullptr) {
        delete listVisualizer;
        listVisualizer = nullptr;
    }
    
    if (linkedList != nullptr) {
        delete linkedList;
        linkedList = nullptr;
    }
}

void DisplayLinkedList() {
    DrawRectangle(0, 0, 400, 100, (Color){245, 162, 178, 255});
    
    if (listVisualizer != nullptr) {
        listVisualizer->draw();
        listVisualizer->handleEvent();
    }

    DrawRectangle(0, 0, 400, 100, (Color){245, 162, 178, 255});
    DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, (Color){255, 254, 206, 255});
}

LinkedListVisualizer::LinkedListVisualizer(LinkedList* list)
    : list(list), mode(MODE_NONE), inputString(""), nodeIndex(-1), selectedNodeIndex(-1),
    manualInputValues(), showFileDialog(false), filePath{0}, fileError(false),
    fileErrorMessage(""), arrowProgress(0.0f), isPaused(false), animationSpeed(1.0f),
    animationProgress(0.0f), operationHistory(), undoHistory(), currentStep(0),
    lastOperation(""), connectionAnimations(), showPseudocode(false), currentPseudocodeLine(0) {}

Operation::Operation(Type t, int idx, int oldVal, int newVal)
    : type(t), nodeIndex(idx), oldValue(oldVal), newValue(newVal) {}

void LinkedListVisualizer::init() {
    mode = MODE_NONE;
    inputString.clear();
    selectedNodeIndex = -1;
    operationHistory.clear();
    undoHistory.clear();
    currentStep = 0;
    animationProgress = 0.0f;
    manualInputValues.clear();
    showFileDialog = false;
    memset(filePath, 0, sizeof(filePath));
    fileError = false;
    fileErrorMessage = "";

    // Seed the random number generator
    srand(static_cast<unsigned>(time(0)));

    // User's instruction
    // lastOperation = "Press M to create a list manually or F to load from a file";
    connectionAnimations.clear();
}

void LinkedListVisualizer::draw() {
    ClearBackground((Color){241, 231, 231, 255});

    float startX = GetScreenWidth() * 0.05f;
    float startY = GetScreenHeight() / 2.f;
    float offsetX = GetScreenWidth() / (list->getSize() + 1);  // Horizontal spacing between nodes

    drawAnimationControls();
    drawOperationInfo();
    
    if (mode == MODE_CREATE_FILE) {
        drawFileUploadInterface();
    } else if (mode == MODE_INITIALIZE) {
        drawInitInterface();
    } else {
        if (list->getHead() == nullptr) {
            // Draw a message prompting the user to create a list
            const char* message = "List is empty. Press A to add nodes or F to load from file.";
            int textWidth = MeasureText(message, 20);
            DrawText(message, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() / 2, 20, (Color){234,179,8, 255});
        } else {
            drawLinkedList(startX, startY, offsetX);
        }
        drawInputBox();
    }
    
    drawHelpText();

    if (showPseudocode) {
        drawPseudocodeBox();
    }
    if (!isPaused) {
        updateAnimation();
    }
}

void LinkedListVisualizer::drawHelpText() {
    const char* helpText = "Controls: I-Init | H-Add at Head | A-Add | D-Delete | U-Update | S-Search | F-File Upload | R-Random | C-Clear | Click node to select";
    int textWidth = MeasureText(helpText, 16);
    DrawText(helpText, (GetScreenWidth() - textWidth) / 2, GetScreenHeight() - 30, 16, (Color){234, 179, 8, 255});
}

void LinkedListVisualizer::drawFileUploadInterface() {
    // Draw a panel for file upload
    int panelWidth = GetScreenWidth() * 0.4f;
    int panelHeight = GetScreenWidth() * 0.4f;
    int panelX = (GetScreenWidth() - panelWidth) / 2;
    int panelY = (GetScreenHeight() - panelHeight) / 2;
    
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, (Color){245, 162, 178, 255});
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, (Color){194, 24, 91, 255});
    
    DrawText("Create Linked List from File", panelX + 20, panelY + 20, 20, (Color){255, 254, 206, 255});
    DrawText("Copy file path here:", panelX + 20, panelY + 60, 18, (Color){255, 254, 206, 255});
    
    // Draw input box
    DrawRectangle(panelX + 20, panelY + 90, panelWidth - 40, 40, (Color){245, 162, 178, 255});
    DrawRectangleLines(panelX + 20, panelY + 90, panelWidth - 40, 40, (Color){194, 24, 91, 255});
    DrawText(filePath, panelX + 30, panelY + 100, 18, (Color){255, 254, 206, 255});
    
    // Draw cursor blink
    static float cursorTimer = 0;
    cursorTimer += GetFrameTime();
    if (fmod(cursorTimer, 1.0f) < 0.5f) {
        float cursorX = panelX + GetScreenWidth() * 0.02f + MeasureText(filePath, 18);
        DrawRectangle(cursorX, panelY + GetScreenHeight() * 0.1f, 2, GetScreenHeight() * 0.02f, (Color){245, 162, 178, 255});
    }
    
    // Show error message if any
    if (fileError) {
        DrawText(fileErrorMessage.c_str(), panelX + 20, panelY + 150, 18, RED);
    }
    
    // Draw buttons
    int buttonWidth = GetScreenWidth() * 0.1f;
    int buttonHeight = GetScreenHeight() * 0.05f;
    int buttonSpacing = 20;
    int buttonsStartX = panelX + (panelWidth - (2 * buttonWidth + buttonSpacing)) / 2;
    int buttonsY = panelY + panelHeight - 60;
    
    // Load button
    bool loadClicked = DrawButton(buttonsStartX, buttonsY, buttonWidth, buttonHeight, "Load File");
    if (loadClicked && strlen(filePath) > 0) {
        createLLFromFile(filePath);
    }
    
    // Browse button (in a real implementation, this would open a file dialog)
    bool browseClicked = DrawButton(buttonsStartX + buttonWidth + buttonSpacing, buttonsY, buttonWidth, buttonHeight, "Browse...");
    if (browseClicked) {
        // In a real implementation, this would open a file dialog
        // For now, we'll just simulate it with a message
        strcpy(filePath, "example.txt");
    }

    // Cancel button
    bool cancelClicked = DrawButton(panelX + panelWidth - 100, panelY + 20, 80, 30, "Cancel");
    if (cancelClicked) {
        mode = MODE_NONE;
        memset(filePath, 0, sizeof(filePath));
        fileError = false;
    }
}

void LinkedListVisualizer::drawInitInterface() {
    // Draw a panel for file upload
    int panelWidth = GetScreenWidth() * 0.4f;
    int panelHeight = GetScreenWidth() * 0.4f;
    int panelX = (GetScreenWidth() - panelWidth) / 2;
    int panelY = (GetScreenHeight() - panelHeight) / 2;
    
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, (Color){245, 162, 178, 255});
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, (Color){194, 24, 91, 255});
    
    DrawText("Initialize Linked List", panelX + 20, panelY + 20, 20, (Color){255, 254, 206, 255});
    DrawText("Enter numbers separated by space:", panelX + 20, panelY + 60, 18, (Color){255, 254, 206, 255});
    
    // Draw input box
    DrawRectangle(panelX + 20, panelY + 90, panelWidth - 40, 40, (Color){245, 162, 178, 255});
    DrawRectangleLines(panelX + 20, panelY + 90, panelWidth - 40, 40, (Color){194, 24, 91, 255});
    DrawText(inputString.c_str(), panelX + 30, panelY + 100, 18, (Color){255, 254, 206, 255});

    static float cursorTimer = 0;
    cursorTimer += GetFrameTime();
    if (fmod(cursorTimer, 1.0f) < 0.5f) {
        float cursorX = panelX + 30 + MeasureText(inputString.c_str(), 18);
        DrawRectangle(cursorX, panelY + 100, 2, 20, BLACK);
    }

    int buttonWidth = GetScreenWidth() * 0.1f;
    int buttonHeight = GetScreenHeight() * 0.05f;
    int buttonSpacing = 20;
    int buttonsStartX = panelX + (panelWidth - (2 * buttonWidth + buttonSpacing)) / 2;
    int buttonsY = panelY + panelHeight - 60;

    // Add button
    bool addClicked = DrawButton(buttonsStartX, buttonsY, buttonWidth, buttonHeight, "Add");
    if (addClicked && !inputString.empty()) {
        std::istringstream iss(inputString);
        int value;
        list->clear();
        while (iss >> value) {
            list->add(value);
        }

        connectionAnimations.clear();
        for (int i = 0; i < list->getSize(); i++) {
            connectionAnimations.push_back({0.0f, animationSpeed});
        }
        
        mode = MODE_NONE;
        inputString.clear();
        lastOperation = "Initialize linked list";
    }

    bool cancelClicked = DrawButton(panelX + panelWidth - 100, panelY + 20, 80, 30, "Cancel");
    if (cancelClicked) {
        mode = MODE_NONE;
        inputString.clear();
    }
}

bool LinkedListVisualizer::createLLFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        fileError = true;
        fileErrorMessage = "Error: Could not open file " + filePath;
        return false;
    }
    
    // Clear the existing list
    list->clear();
    
    // Read values from file
    std::vector<int> values;
    int value;
    while (file >> value) {
        values.push_back(value);
    }
    
    // Check if we got any values
    if (values.empty()) {
        fileError = true;
        fileErrorMessage = "Error: No valid integers found in file";
        return false;
    }
    
    // Add all the values to the list
    for (int val : values) {
        list->add(val);
    }
    
    // Reset the visualizer state
    mode = MODE_NONE;
    memset(this->filePath, 0, sizeof(this->filePath));
    fileError = false;
    operationHistory.clear();
    undoHistory.clear();
    currentStep = 0;
    animationProgress = 0.0f;
    lastOperation = "Created list from file: " + filePath;

    connectionAnimations.clear();
    for (size_t i = 0; i < values.size() - 1; ++i) {
        connectionAnimations.push_back({0.0f, animationSpeed});
    }

    return true;
}

void LinkedListVisualizer::drawPseudocodeBox() {
    int boxWidth = GetScreenWidth() * 0.4f;
    int boxHeight = GetScreenHeight() * 0.3f;
    int boxX = (GetScreenWidth() - boxWidth) / 2;
    int boxY = GetScreenHeight() * 0.7f; // Position the box below the linked list

    // Draw the box
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, (Color){245, 162, 178, 255});
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, (Color){245, 162, 178, 255});

    // Display pseudocode based on the current operation
    const char* pseudocodeLines[10] = {nullptr};
    int lineCount = 0;
    if (currentStep < static_cast<int>(operationHistory.size())) {
        const Operation& op = operationHistory[currentStep];
        switch (op.type) {
            case Operation::ADD:
                pseudocodeLines[0] = "ADD Operation:\n";
                pseudocodeLines[1] = "1. Create a new node.\n";
                pseudocodeLines[2] = "2. Set the value of the new node.\n";
                pseudocodeLines[3] = "3. If the list is empty:\n";
                pseudocodeLines[4] = "   a. Set head to the new node.\n";
                pseudocodeLines[5] = "4. Else:\n";
                pseudocodeLines[6] = "   a. Traverse to the last node.\n";
                pseudocodeLines[7] = "   b. Link the last node to the new node.";
                lineCount = 8;
                break;
            case Operation::DELETE:
                pseudocodeLines[0] = "DELETE Operation:\n";
                pseudocodeLines[1] = "1. If the list is empty:\n";
                pseudocodeLines[2] = "   a. Print 'List is empty.'\n";
                pseudocodeLines[3] = "   b. Exit.\n";
                pseudocodeLines[4] = "2. Find the node at the given index:\n";
                pseudocodeLines[5] = "   a. Traverse the list to the index.\n";
                pseudocodeLines[6] = "   b. Keep track of the previous node.\n";
                pseudocodeLines[7] = "3. Remove the node:\n";
                pseudocodeLines[8] = "   a. Update the previous node's link to skip the target node.\n";
                lineCount = 9;
                break;
            case Operation::UPDATE:
                pseudocodeLines[0] = "UPDATE Operation:\n";
                pseudocodeLines[1] = "1. If the list is empty:\n";
                pseudocodeLines[2] = "   a. Print 'List is empty.'\n";
                pseudocodeLines[3] = "   b. Exit.\n";
                pseudocodeLines[4] = "2. Find the node at the given index:\n";
                pseudocodeLines[5] = "   a. Traverse the list to the index.\n";
                pseudocodeLines[6] = "3. Update the value of the node.";
                break;
            case Operation::SEARCH:
                pseudocodeLines[0] = "SEARCH Operation:\n";
                pseudocodeLines[1] = "1. If the list is empty:\n";
                pseudocodeLines[2] = "   a. Print 'List is empty.'\n";
                pseudocodeLines[3] = "   b. Exit.\n";
                pseudocodeLines[4] = "2. Traverse the list:\n";
                pseudocodeLines[5] = "   a. Compare each node's value with the target value.\n";
                pseudocodeLines[6] = "3. If found:\n";
                pseudocodeLines[7] = "   a. Print 'Value found at index X.'\n";
                pseudocodeLines[8] = "4. Else:\n";
                pseudocodeLines[9] = "   a. Print 'Value not found.'";
                lineCount = 10;
                break;
            default:
                pseudocodeLines[0] = "No operation selected.";
                lineCount = 1;
                break;
        }
    } else {
        pseudocodeLines[0] = "No operation selected.";
        lineCount = 1;
    }

    // Draw the pseudocode text
    int textX = boxX + 20;
    int textY = boxY + 20;
    int lineSpacing = 20;
    for (int i = 0; i < lineCount; i++) {
        Color textColor = (i == currentPseudocodeLine) ? (Color){208, 135, 112, 255} : (Color){255, 254, 206, 255}; // Highlight current line
        DrawText(pseudocodeLines[i], textX, textY + i * lineSpacing, 18, textColor);
    }
}

void LinkedListVisualizer::drawAnimationControls() {
    float controlsY = GetScreenHeight() - 80;
    float buttonWidth = 30;
    float buttonSpacing = 10;
    float startX = GetScreenWidth() * 0.25 - (buttonWidth * 4 + buttonSpacing * 3) / 2;

    // Step backward button ("<<")
    if (DrawButton(startX, controlsY, buttonWidth, buttonWidth, "<<")) {
        stepBackward(); // Step backward through operations
    }
    
    // Undo button
    if (DrawButton(startX + buttonWidth + buttonSpacing, controlsY, buttonWidth, buttonWidth, "<")) {
        if (!operationHistory.empty()) {
            Operation lastOp = operationHistory.back();
            operationHistory.pop_back();
            undoHistory.push_back(lastOp);
            undoOperation(lastOp);
            lastOperation = "Undid operation: " + lastOp.toString();
        }
    }

    // Play button
    if (DrawButton(startX + (buttonWidth + buttonSpacing) * 2, controlsY, buttonWidth, buttonWidth, "||")) {
        isPaused = false;
    }

    // Pause button
    if (DrawButton(startX + (buttonWidth + buttonSpacing) * 3, controlsY, buttonWidth, buttonWidth, "|>")) {
        isPaused = true;
    }
    
    // Redo button
    if (DrawButton(startX + (buttonWidth + buttonSpacing) * 4, controlsY, buttonWidth, buttonWidth, ">")) {
        if (!undoHistory.empty()) {
            // Redo the last undone operation
            Operation lastUndo = undoHistory.back();
            undoHistory.pop_back();
            operationHistory.push_back(lastUndo);
            applyOperation(lastUndo);
            lastOperation = "Redid operation: " + lastUndo.toString();
        }
    }

    // Step forward button (">>")
    if (DrawButton(startX + (buttonWidth + buttonSpacing) * 5, controlsY, buttonWidth, buttonWidth, ">>")) {
        stepForward(); // Step forward through operations
    }

    // Speed control slider
    float sliderWidth = 200;
    float sliderX = GetScreenWidth() * 0.6;
    DrawText("Speed:", sliderX, controlsY, 20, (Color){234, 179, 8, 255});
    Rectangle sliderRect = { sliderX + 110, controlsY, sliderWidth, 20 };
    // Update animation speed dynamically
    animationSpeed = GuiSlider(sliderRect, "0.5x", "2.0x", animationSpeed, 0.5f, 2.0f);

    // Toggle pseudocode box button (lower-right corner)
    float toggleButtonX = sliderX + sliderWidth + 20; // Position to the right of the slider
    float toggleButtonY = controlsY - 60;            // Slightly above the slider
    const char* toggleButtonText = showPseudocode ? ">" : "<";
    if (DrawButton(toggleButtonX, toggleButtonY, buttonWidth, buttonWidth, toggleButtonText)) {
        showPseudocode = !showPseudocode; // Toggle the pseudocode box
    }
}

std::string Operation::toString() {
    switch (type) {
        case ADD: return "Add node with value " + std::to_string(newValue);
        case DELETE: return "Delete node at index " + std::to_string(nodeIndex);
        case UPDATE: return "Update node at index " + std::to_string(nodeIndex) +
                             " from " + std::to_string(oldValue) + " to " + std::to_string(newValue);
        case SEARCH: return "Search for value " + std::to_string(newValue);
        default: return "Unknown operation";
    }
}

void LinkedListVisualizer::drawOperationInfo() {
    int screenWidth = GetScreenWidth();   
    int rightMargin = 10;
    int topMargin = 10;
    int rectHeight = 30;
    int rectWidth = 200;
    DrawRectangle(screenWidth - rectWidth - rightMargin, topMargin, rectWidth, rectHeight,  (Color){245, 162, 178, 255});
    string stepInfo = "Step " + to_string(currentStep) + " of " + to_string(operationHistory.size());
    DrawText(stepInfo.c_str(), screenWidth - rectWidth - rightMargin + 10, topMargin + 5, 20, (Color){255, 254, 206, 255});

    string modeText = "Mode: ";
    switch(mode) {
        case MODE_ADD: modeText += "Add Node"; break;
        case MODE_DELETE: modeText += "Delete Node"; break;
        case MODE_UPDATE: modeText += "Update Node"; break;
        case MODE_SEARCH: modeText += "Search"; break;
        default: modeText += "None"; break;
    }
    DrawText(modeText.c_str(), screenWidth - rectWidth - rightMargin + 10, topMargin + rectHeight + 5, 18, (Color){234, 179, 8, 255});
   
    // Display last operation
    if (!lastOperation.empty()) {
        DrawText(lastOperation.c_str(), screenWidth - rectWidth - rightMargin + 10, topMargin + rectHeight + 30, 16, (Color){234, 179, 8, 255});
    }
}

void LinkedListVisualizer::drawLinkedList(float startX, float startY, float offsetX) {
    Node* current = list->getHead();
    int index = 0;
    
    // First pass: Draw connections
    while (current != nullptr && current->next != nullptr) {
        float posX = startX + index * offsetX;
        float posY = startY;
        if (index < static_cast<int>(connectionAnimations.size())) {
            drawConnection(posX, posY, offsetX, index);
        }
        current = current->next;
        index++;
    }
    
    // Second pass: Draw nodes on top
    current = list->getHead();
    index = 0;
    
    while (current != nullptr) {
        float posX = startX + index * offsetX;
        float posY = startY;
        
        int animationNodeIndex = -1;
        if (!operationHistory.empty() && currentStep < static_cast<int>(operationHistory.size())) {
            animationNodeIndex = operationHistory[currentStep].nodeIndex;
        }
        
        if (index == animationNodeIndex) {
            applyAnimationEffects(posX, posY, current, index);
        } else {
            drawNode(posX, posY, current, index);
        }
        
        current = current->next;
        index++;
    }
}

void LinkedListVisualizer::drawNode(float posX, float posY, Node* node, int index) {
    Color nodeColor = (Color){245, 162, 178, 255};
    
    // Selected node
    if (index == selectedNodeIndex) {
        nodeColor = {245, 162, 178, 255};
    } 
    // Search highlighted node
    else if (mode == MODE_SEARCH && !inputString.empty() && 
             to_string(node->val) == inputString) {
        nodeColor = {163, 190, 140, 255};
    }
    
    DrawCircle(posX, posY, 30.f, nodeColor);
    DrawCircleLines(posX, posY, 30.f, (Color){194, 24, 91, 255});
    
    // Draw node value
    const char* valueText = TextFormat("%d", node->val);
    float textWidth = MeasureText(valueText, 20);
    DrawText(valueText, posX - textWidth/2, posY - 10, 20, BLACK);
    
    // Draw index
    const char* indexText = TextFormat("%d", index);
    float indexWidth = MeasureText(indexText, 16);
    DrawText(indexText, posX - indexWidth/2, posY - 50, 16, BLACK);
}

void LinkedListVisualizer::drawConnection(float startX, float startY, float offsetX, int connectionIndex) {
    float arrowEndX = startX + offsetX - GetScreenWidth() * 0.02f;
    float arrowStartX = startX + GetScreenWidth() * 0.02f;
     // Get the progress and speed for this connection
    auto& [arrowProgress, speed] = connectionAnimations[connectionIndex];

    // Increment progress only if it hasn't reached the end
    if (arrowProgress < 1.0f) {
        arrowProgress += GetFrameTime() * animationSpeed;
        if (arrowProgress > 1.0f) {
            arrowProgress = 1.0f; // Clamp to 1.0 to stop the animation
        }
    }

    // Interpolate the arrow's position based on progress
    float currentArrowX = arrowStartX + (arrowEndX - arrowStartX) * arrowProgress;

    // Draw the line up to the current arrow position
    DrawLine(startX + 30, startY, currentArrowX, startY, BLACK);

    // Draw the arrow tip at the current position
    if (arrowProgress >= 1.0f) {
        // Draw the full arrow tip when the animation is complete
        Vector2 arrowTip = {arrowEndX, startY};
        Vector2 arrowLeft = {arrowEndX - 10, startY - 5};
        Vector2 arrowRight = {arrowEndX - 10, startY + 5};
        DrawTriangle(arrowTip, arrowLeft, arrowRight, BLACK);
    } else {
        // Draw the arrow tip at the current position during animation
        Vector2 arrowTip = {currentArrowX, startY};
        Vector2 arrowLeft = {currentArrowX - 10, startY - 5};
        Vector2 arrowRight = {currentArrowX - 10, startY + 5};
        DrawTriangle(arrowTip, arrowLeft, arrowRight, BLACK);
    }
}

void LinkedListVisualizer::drawInputBox() {
    int screenWidth = GetScreenWidth();
    int rightMargin = 10;
    int topMargin = 50;
    int rectWidth = 300;
    int rectHeight = 35;

    int rectX = screenWidth - rectWidth - rightMargin;
    int rectY = topMargin + 40;

    DrawRectangle(rectX, rectY, rectWidth, rectHeight, (Color){245, 162, 178, 255});
    DrawRectangleLines(rectX, rectY, rectWidth, rectHeight, (Color){194, 24, 91, 255} );
    
    // Show appropriate prompt based on mode
    string prompt = "";
    switch(mode) {
        case MODE_ADD_HEAD: prompt = "Enter value to add at head: "; break;
        case MODE_ADD: prompt = "Enter value to add: "; break;
        case MODE_UPDATE: 
            prompt = "Enter new value for node " + to_string(selectedNodeIndex) + ": "; 
            break;
        case MODE_SEARCH: prompt = "Enter value to search: "; break;
        default: break;
    }
    
    int textX = rectX + 10;  // Padding from the left
    int textY = rectY + 15; 
    DrawText(prompt.c_str(), textX, textY, 18, (Color){255, 254, 206, 255});
    DrawText(inputString.c_str(), textX + MeasureText(prompt.c_str(), 18), textY, 18, (Color){255, 254, 206, 255});
    
    // Draw cursor blink
    static float cursorTimer = 0;
    cursorTimer += GetFrameTime();
    if (fmod(cursorTimer, 1.0f) < 0.5f && (mode == MODE_ADD || mode == MODE_UPDATE || mode == MODE_SEARCH)) {
        float cursorX = textX + MeasureText((prompt + inputString).c_str(), 18);
        DrawRectangle(cursorX, textY, 2, 18, BLACK);
    }
}

void LinkedListVisualizer::handleEvent() {
    // Handle mode selection
    if (IsKeyPressed(KEY_I)) {
        mode = MODE_INITIALIZE;
        inputString.clear();
    } else if (IsKeyPressed(KEY_A)) {
        mode = MODE_ADD;
        inputString = "";
    } else if (IsKeyPressed(KEY_D)) {
        mode = MODE_DELETE;
        selectedNodeIndex = -1; // wait for node selection
    } else if (IsKeyPressed(KEY_U)) {
        mode = MODE_UPDATE;
        inputString = "";
        selectedNodeIndex = -1; // wait for node selection
    } else if (IsKeyPressed(KEY_S)) {
        mode = MODE_SEARCH;
        inputString = "";
    } else if (IsKeyPressed(KEY_F)) {
        mode = MODE_CREATE_FILE;
        memset(filePath, 0, sizeof(filePath));
        fileError = false;
    } else if (IsKeyPressed(KEY_R)) {
        list->clear();

        for (int i = 0; i < 10; i++) {
            int randomValue = rand() % 100;
            list->add(randomValue);
        }

        lastOperation = "Generate random linked list with 10 nodes";
        operationHistory.clear();
        undoHistory.clear();
        currentStep = 0;
        animationProgress = 0.0f;

        connectionAnimations.clear();
        for (int i = 0; i < 9; i++) {
            connectionAnimations.push_back({0.0f, animationSpeed});
        }
    } else if (IsKeyPressed(KEY_C)) {
        list->clear();
        lastOperation = "Clear the existing linked list";
        operationHistory.clear();
        undoHistory.clear();
        currentStep = 0;
        animationProgress = 0.0f;

        connectionAnimations.clear();
    } else if (IsKeyPressed(KEY_H)) {
        mode = MODE_ADD_HEAD;
        inputString.clear();
    }

    // Handle text input
    if (mode == MODE_ADD || mode == MODE_UPDATE || mode == MODE_SEARCH || mode == MODE_INITIALIZE) {
        int key = GetCharPressed();
        if (key >= '0' && key <= '9' || key == ' ') {
            inputString += (char)key;
        }
        
        if (IsKeyPressed(KEY_BACKSPACE) && !inputString.empty()) {
            inputString.pop_back();
        }
    } else if (mode == MODE_CREATE_FILE) {
        int key = GetCharPressed();
        // Allow alphanumeric characters, dots, slashes, and backslashes for file paths
        if ((key >= 32 && key <= 126)) {  // Printable ASCII characters
            size_t len = strlen(filePath);
            if (len < sizeof(filePath) - 1) {
                filePath[len] = (char)key;
                filePath[len + 1] = '\0';
            }
        }
        
        if (IsKeyPressed(KEY_BACKSPACE) && strlen(filePath) > 0) {
            filePath[strlen(filePath) - 1] = '\0';
        }

        // Handle Ctrl+V for pasting from clipboard
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_V)) {
            const char* clipboardText = GetClipboardText();
            if (clipboardText != nullptr) {
                size_t len = strlen(filePath);
                size_t clipboardLen = strlen(clipboardText);
                size_t availableSpace = sizeof(filePath) - len - 1;

                // Append clipboard text to the file path, ensuring it doesn't exceed the buffer size
                strncat(filePath, clipboardText, availableSpace);
            }
        }

        // Handle Ctrl+C for copying the current file path to the clipboard
        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_C)) {
            SetClipboardText(filePath);
        }
    } else if (mode == MODE_ADD_HEAD) {
        int key = GetCharPressed();
        if (key >= '0' && key <= '9') {
            inputString += (char)key;
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !inputString.empty()) {
            inputString.pop_back();
        }
        if (IsKeyPressed(KEY_ENTER) && !inputString.empty()) {
            try {
                int value = std::stoi(inputString);
                list->addAtHead(value); // Add node at head
                lastOperation = "Added node with value " + inputString + " at head";

                // Update connection animations
                connectionAnimations.insert(connectionAnimations.begin(), {0.0f, animationSpeed});

                inputString.clear();
                mode = MODE_NONE; // Reset mode
            } catch (std::exception& e) {
                lastOperation = "Invalid input: " + std::string(e.what());
            }
        }
    }

    // Handle node selection via mouse
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        float startX = 50.f;
        float startY = GetScreenHeight() / 2.f;
        float offsetX = GetScreenWidth() / (list->getSize() + 1);;
        int index = 0;
        Node* current = list->getHead();
        
        while (current != nullptr) {
            float posX = startX + index * offsetX;
            float posY = startY;
            
            float nodeRadius = GetScreenHeight() * 0.04f;
            if (CheckCollisionPointCircle(mousePos, (Vector2){posX, posY}, nodeRadius)) {
                selectedNodeIndex = index;
                
                // If in delete mode, handle delete operation immediately
                if (mode == MODE_DELETE) {
                    // Create delete operation
                    Operation op(Operation::DELETE, index, current->val, 0);
                    operationHistory.push_back(op);
                    
                    // Perform delete
                    if (list->deleteAt(index)) {
                        lastOperation = "Deleted node at index " + to_string(index);
                        if (index < static_cast<int>(connectionAnimations.size())) {
                            connectionAnimations.erase(connectionAnimations.begin() + index);
                        }
                    } else {
                        lastOperation = "Failed to delete node at index " + to_string(index);
                    }
                    
                    // Reset mode and selection
                    mode = MODE_NONE;
                    selectedNodeIndex = -1;
                    
                    // Reset animation to current step
                    currentStep = operationHistory.size() - 1;
                    animationProgress = 0.0f;
                }
                break;
            }
            current = current->next;
            ++index;
        }
    }

    // Handle operation confirmation
    if (IsKeyPressed(KEY_ENTER)) {
        if (mode == MODE_ADD && !inputString.empty()) {
            try {
                int value = std::stoi(inputString);
                
                // Create operation and add to history
                Operation op(Operation::ADD, list->getSize(), 0, value);
                operationHistory.push_back(op);
                
                // Perform add
                list->add(value);
                lastOperation = "Added node with value: " + inputString;
                
                // Add a new connection animation with the current speed
                connectionAnimations.push_back({0.0f, animationSpeed});

                // Reset and update animation
                inputString = "";
                mode = MODE_NONE;
                currentStep = operationHistory.size() - 1;
                animationProgress = 0.0f;
            } catch (std::exception& e) {
                lastOperation = "Invalid input: " + string(e.what());
            }
        } else if (mode == MODE_UPDATE && selectedNodeIndex != -1 && !inputString.empty()) {
            try {
                int value = std::stoi(inputString);
                
                // Get current value at the index
                int oldValue = list->getAt(selectedNodeIndex);
                
                // Create operation and add to history
                Operation op(Operation::UPDATE, selectedNodeIndex, oldValue, value);
                operationHistory.push_back(op);
                
                // Perform update
                list->update(selectedNodeIndex, value);
                lastOperation = "Updated node at index " + to_string(selectedNodeIndex) + 
                              " to value: " + inputString;
                
                // Reset and update animation
                inputString = "";
                mode = MODE_NONE;
                selectedNodeIndex = -1;
                currentStep = static_cast<int>(operationHistory.size()) - 1;
                animationProgress = 0.0f;
            } catch (std::exception& e) {
                lastOperation = "Invalid input: " + string(e.what());
            }
        } else if (mode == MODE_SEARCH && !inputString.empty()) {
            try {
                int value = std::stoi(inputString);
                int foundIndex = list->search(value);
                
                if (foundIndex != -1) {
                    selectedNodeIndex = foundIndex;
                    lastOperation = "Found value " + inputString + " at index " + to_string(foundIndex);
                } else {
                    lastOperation = "Value " + inputString + " not found in list";
                }
                
                // Create operation and add to history
                Operation op(Operation::SEARCH, foundIndex, 0, value);
                operationHistory.push_back(op);
                
                // Reset and update animation
                mode = MODE_NONE;
                currentStep = operationHistory.size() - 1;
                animationProgress = 0.0f;
            } catch (std::exception& e) {
                lastOperation = "Invalid input: " + string(e.what());
            }
        }
    }
}

void LinkedListVisualizer::updateAnimation() {    
    if (!operationHistory.empty() && currentStep < static_cast<int>(operationHistory.size())) {
        animationProgress += GetFrameTime() * animationSpeed;
        if (animationProgress >= 1.0f) {
            animationProgress = 0.0f;
            // Don't automatically advance to next step
            // This gives users time to see the completed operation
        }
    }
}

void LinkedListVisualizer::applyOperation(const Operation& op) {
    switch (op.type) {
        case Operation::ADD:
            list->add(op.newValue);
            break;
        case Operation::DELETE:
            list->deleteAt(op.nodeIndex);
            break;
        case Operation::UPDATE:
            list->update(op.nodeIndex, op.newValue);
            break;
        case Operation::SEARCH:
            // Handle search visualization
            break;
    }
}

void LinkedListVisualizer::undoOperation(const Operation& op) {
    switch (op.type) {
        case Operation::ADD:
            list->deleteAt(op.nodeIndex);
            break;
        case Operation::DELETE:
            if (op.nodeIndex <= list->getSize()) {
                list->insertAt(op.nodeIndex, op.oldValue);
            }
            break;
        case Operation::UPDATE:
            list->update(op.nodeIndex, op.oldValue);
            break;
        case Operation::SEARCH:
            // No need to undo search operation
            break;
    }
}

void LinkedListVisualizer::stepForward() {
    if (currentStep < static_cast<int>(operationHistory.size())) {
        currentStep++;
        animationProgress = 0.0f; // Reset animation progress
    }
}

void LinkedListVisualizer::stepBackward() {
    if (currentStep > 0) {
        currentStep--;
        animationProgress = 0.0f; // Reset animation progress
    }
}

void LinkedListVisualizer::applyAnimationEffects(float posX, float posY, Node* node, int index) {
    const Operation& currentOp = operationHistory[currentStep];
    
    switch (currentOp.type) {
        case Operation::ADD:
            if (index == currentOp.nodeIndex) {
                // Highlight pseudocode lines based on animation progress
                if (animationProgress < 0.2f) {
                    currentPseudocodeLine = 0; // "1. Create a new node."
                } else if (animationProgress < 0.4f) {
                    currentPseudocodeLine = 1; // "2. Set the value of the new node."
                } else if (animationProgress < 0.6f) {
                    currentPseudocodeLine = 2; // "3. If the list is empty:"
                } else if (animationProgress < 0.8f) {
                    currentPseudocodeLine = 5; // "4. Else: Traverse to the last node."
                } else {
                    currentPseudocodeLine = 6; // "   b. Link the last node to the new node."
                }

                // Fade in effect for new node
                Color nodeColor = {245, 162, 178, 255};
                float alpha = animationProgress;
                DrawCircle(posX, posY, 30.f, ColorAlpha(nodeColor, alpha));
                DrawCircleLines(posX, posY, 30.f, (Color){194, 24, 91, 255} );
                
                const char* valueText = TextFormat("%d", node->val);
                float textWidth = MeasureText(valueText, 20);
                DrawText(valueText, posX - textWidth/2, posY - 10, 20, ColorAlpha(BLACK, alpha));
            }
            break;
            
        case Operation::DELETE:
            if (index == currentOp.nodeIndex) {
                // Highlight pseudocode lines based on animation progress
                if (animationProgress < 0.3f) {
                    currentPseudocodeLine = 3; // "2. Find the node at the given index:"
                } else {
                    currentPseudocodeLine = 6; // "3. Remove the node:"
                }

                // Fade out effect for deleted node
                Color nodeColor = {208, 135, 112, 255};
                float alpha = 1.0f - animationProgress;
                DrawCircle(posX, posY, 30.f, ColorAlpha(nodeColor, alpha));
                DrawCircleLines(posX, posY, 30.f, ColorAlpha(BLACK, alpha));
                
                const char* valueText = TextFormat("%d", node->val);
                float textWidth = MeasureText(valueText, 20);
                DrawText(valueText, posX - textWidth/2, posY - 10, 20, ColorAlpha(BLACK, alpha));
            }
            break;
            
        case Operation::UPDATE:
            if (index == currentOp.nodeIndex) {
                // Highlight pseudocode lines based on animation progress
                if (animationProgress < 0.5f) {
                    currentPseudocodeLine = 4; // "2. Find the node at the given index:"
                } else {
                    currentPseudocodeLine = 6; // "3. Update the value of the node."
                }

                // Transition effect for updated value
                DrawCircle(posX, posY, 30.f, (Color){ 124, 156, 191, 255 });
                DrawCircleLines(posX, posY, 30.f, WHITE);
                
                const char* valueText = TextFormat("%d", currentOp.newValue);
                float textWidth = MeasureText(valueText, 20);
                DrawText(valueText, posX - textWidth/2, posY - 10, 20, BLACK);
            }
            break;
            
        case Operation::SEARCH:
            if (node->val == currentOp.newValue) {
                // Highlight pseudocode lines based on animation progress
                if (animationProgress < 0.5f) {
                    currentPseudocodeLine = 4; // "2. Traverse the list:"
                } else {
                    currentPseudocodeLine = 6; // "3. If found: Print 'Value found at index X.'"
                }

                // Pulse effect for found node
                Color nodeColor = {163, 190, 140, 255};
                float pulse = 0.5f + 0.5f * sin(animationProgress * PI * 4);
                DrawCircle(posX, posY, 30.f + pulse * 5.f, nodeColor);
                DrawCircleLines(posX, posY, 30.f + pulse * 5.f, WHITE);
                
                const char* valueText = TextFormat("%d", node->val);
                float textWidth = MeasureText(valueText, 20);
                DrawText(valueText, posX - textWidth/2, posY - 10, 20, BLACK);
            } else {
                // Draw regular node
                drawNode(posX, posY, node, index);
            }
            break;
    }
}

bool LinkedListVisualizer::DrawButton(float x, float y, float width, float height, const char* text) {
    Rectangle btnRect = { x, y, width, height };
    bool clicked = false;
    
    if (CheckCollisionPointRec(GetMousePosition(), btnRect)) {
        DrawRectangleRec(btnRect, (Color){245, 162, 178, 255});
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            clicked = true;
        }
    } else {
        DrawRectangleRec(btnRect, (Color){245, 162, 178, 255});
    }
    
    DrawRectangleLinesEx(btnRect, 1, (Color){194, 24, 91, 255});
    
    float textWidth = MeasureText(text, 20);
    float textX = x + (width - textWidth) / 2;
    float textY = y + (height - 20) / 2;
    DrawText(text, textX, textY, 20, (Color){255, 254, 206, 255});
    
    return clicked;
}

float LinkedListVisualizer::GuiSlider(Rectangle bounds, const char* textLeft, const char* textRight, 
    float value, float minValue, float maxValue) {
    // Draw slider background
    DrawRectangleRec(bounds, (Color){245, 162, 178, 255});
    DrawRectangleLinesEx(bounds, 1, (Color){194, 24, 91, 255});

    // Calculate slider position based on current value
    float range = maxValue - minValue;
    float sliderPos = bounds.x + (bounds.width * ((value - minValue) / range));

    // Draw slider knob
    DrawCircle(sliderPos, bounds.y + bounds.height/2, 10, (Color){255, 254, 206, 255});
    DrawCircleLines(sliderPos, bounds.y + bounds.height/2, 10, (Color){194, 24, 91, 255});

    // Draw min/max labels
    DrawText(textLeft, bounds.x - MeasureText(textLeft, 16) - 5, 
    bounds.y + bounds.height/2 - 8, 16, (Color){234, 179, 8, 255});
    DrawText(textRight, bounds.x + bounds.width + 5, 
    bounds.y + bounds.height/2 - 8, 16, (Color){234, 179, 8, 255});

    // Handle mouse input
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, bounds)) {
            float newValue = ((mousePos.x - bounds.x) / bounds.width) * range + minValue;
            // Clamp value between min and max
            if (newValue < minValue) newValue = minValue;
            if (newValue > maxValue) newValue = maxValue;
            return newValue;
        }
    }

    return value;
}

void LINKEDLIST_INTERACT() {
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if(MouseButtonPressed(0, 0, 400, 100)) {
            currentScreen = MAINMENU;
            CleanupLinkedList();
        }
    }
}

#endif // LINKEDLISTVISUALIZER0_H
