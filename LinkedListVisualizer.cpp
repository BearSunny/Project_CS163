#include "LinkedListVisualizer.h"
#include <sstream>
#include <raylib.h>
#include <cmath>

LinkedListVisualizer::LinkedListVisualizer(LinkedList* list)
    : list(list), mode(MODE_NONE), inputString(""), nodeIndex(-1),
    isPaused(false), animationSpeed(1.0f), animationProgress(0.0f),
    currentStep(0), lastOperation("") {}

Operation::Operation(Type t, int idx, int oldVal, int newVal)
    : type(t), nodeIndex(idx), oldValue(oldVal), newValue(newVal) {}

void LinkedListVisualizer::init() {
    mode = MODE_NONE;
    inputString.clear();
    nodeIndex = -1;
    operationHistory.clear();
    undoHistory.clear();
    currentStep = 0;
    animationProgress = 0.0f;
}

void LinkedListVisualizer::draw() {
    float startX = 50.f;
    float startY = GetScreenHeight() / 2.f;
    float offsetX = 100.f;  // Horizontal spacing between nodes

    drawAnimationControls();
    drawOperationInfo();
    drawLinkedList(startX, startY, offsetX);
    drawInputBox();
    drawHelpText();

    if (!isPaused) {
        updateAnimation();
    }
}

void LinkedListVisualizer::drawAnimationControls() {
    float controlsY = GetScreenHeight() - 80;
    float buttonWidth = 30;
    float buttonSpacing = 10;
    float startX = GetScreenWidth() / 2 - (buttonWidth * 3 + buttonSpacing * 2) / 2;

    // Backward button
    if (DrawButton(startX, controlsY, buttonWidth, buttonWidth, "<")) {
        stepBackward();
    }

    // Play/Pause button
    if (DrawButton(startX + buttonWidth + buttonSpacing, controlsY, buttonWidth, buttonWidth, 
                  isPaused ? "▶" : "⏸")) {
        isPaused = !isPaused;
    }

    // Forward button
    if (DrawButton(startX + (buttonWidth + buttonSpacing) * 2, controlsY, buttonWidth, buttonWidth, ">")) {
        stepForward();
    }

    // Speed control slider
    float sliderWidth = 100;
    float sliderX = startX + (buttonWidth + buttonSpacing) * 3 + 20;
    DrawText("Speed:", sliderX, controlsY, 20, DARKGRAY);
    Rectangle sliderRect = { sliderX + 70, controlsY + 5, sliderWidth, 20 };
    animationSpeed = GuiSlider(sliderRect, "0.5x", "2.0x", animationSpeed, 0.5f, 2.0f);
}

void LinkedListVisualizer::drawOperationInfo() {
    DrawRectangle(50, 10, 300, 30, DARKGRAY);
    string stepInfo = "Step " + to_string(currentStep) + " of " + to_string(operationHistory.size());
    DrawText(stepInfo.c_str(), 60, 15, 18, WHITE);

    string modeText = "Mode: ";
    switch(mode) {
        case MODE_ADD: modeText += "Add Node"; break;
        case MODE_DELETE: modeText += "Delete Node"; break;
        case MODE_UPDATE: modeText += "Update Node"; break;
        case MODE_SEARCH: modeText += "Search"; break;
        default: modeText += "None"; break;
    }
    DrawText(modeText.c_str(), GetScreenWidth() - 200, 15, 18, WHITE);
}

void LinkedListVisualizer::drawLinkedList(float startX, float startY, float offsetX) {
    Node* current = list->getHead();
    int index = 0;

    while (current != nullptr) {
        float posX = startX + index * offsetX;
        float posY = startY;

        if (index == nodeIndex && !operationHistory.empty()) {
            applyAnimationEffects(posX, posY, current, index);
        }

        drawNode(posX, posY, current, index);
        if (current->next != nullptr) {
            drawConnection(posX, posY, offsetX);
        }

        current = current->next;
        index++;
    }
}

void LinkedListVisualizer::drawNode(float posX, float posY, Node* node, int index) {
    Color nodeColor = WHITE;
    if (index == nodeIndex) {
        nodeColor = BLUE;
    } else if (mode == MODE_SEARCH && !inputString.empty() && 
               to_string(node->val) == inputString) {
        nodeColor = GREEN;
    }
    
    DrawCircle(posX, posY, 30.f, nodeColor);
    DrawText(TextFormat("%d", node->val), posX - 10, posY - 10, 20, BLACK);
    DrawText(TextFormat("%d", index), posX - 10, posY - 50, 16, GRAY);
}

void LinkedListVisualizer::drawConnection(float startX, float startY, float offsetX) {
    float arrowEndX = startX + offsetX - 30;
    float arrowStartX = startX + 30;
    
    DrawLine(arrowStartX, startY, arrowEndX, startY, BLACK);
    Vector2 arrowTip = {arrowEndX, startY};
    Vector2 arrowLeft = {arrowEndX - 10, startY - 5};
    Vector2 arrowRight = {arrowEndX - 10, startY + 5};
    DrawTriangle(arrowTip, arrowLeft, arrowRight, BLACK);
}

void LinkedListVisualizer::drawInputBox() {
    DrawRectangle(50, 50, 300, 50, DARKGRAY);
    DrawText(inputString.c_str(), 60, 60, 18, WHITE);
}

void LinkedListVisualizer::drawHelpText() {
    DrawText("Controls: I-Init | A-Add | D-Delete | U-Update | S-Search | Click node to select", 
             50, GetScreenHeight() - 30, 16, DARKGRAY);
}

void LinkedListVisualizer::handleEvent() {
    if (IsKeyPressed(KEY_I)) {
        mode = MODE_INITIALIZE;
        inputString = "";
    } else if (IsKeyPressed(KEY_A)) {
        mode = MODE_ADD;
        inputString = "";
    } else if (IsKeyPressed(KEY_D)) {
        mode = MODE_DELETE;
        nodeIndex = -1; // wait for node selection
    } else if (IsKeyPressed(KEY_U)) {
        mode = MODE_UPDATE;
        inputString = "";
        nodeIndex = -1; // wait for node selection
    } else if (IsKeyPressed(KEY_S)) {
        mode = MODE_SEARCH;
        inputString = "";
    }

    if (mode == MODE_ADD || mode == MODE_UPDATE || mode == MODE_SEARCH) {
        int key = GetKeyPressed();
        if (key >= '0' && key <= '9') {
            inputString += (char)key;
        } else if (key == KEY_BACKSPACE && !inputString.empty()) {
            inputString.pop_back();
        }
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        float startX = 50.f;
        float startY = GetScreenHeight() / 2.f;
        float offsetX = 100.f;
        int index = 0;
        Node* current = list->getHead();
        while (current != nullptr) {
            float posX = startX + index * offsetX;
            float posY = startY;
            if (CheckCollisionPointCircle(mousePos, (Vector2){posX, posY}, 30.f)) {
                nodeIndex = index;
                if (mode == MODE_DELETE) {
                    list->deleteAt(index); // backend function, update later
                    nodeIndex = -1;
                }
                break;
            }
            current = current->next;
            ++index;
        }
    }

    if (IsKeyPressed(KEY_ENTER)) {
        if (mode == MODE_ADD && !inputString.empty()) {
            int value = std::stoi(inputString);
            Operation op(Operation::ADD, list->getSize(), 0, value);
            operationHistory.push_back(op);
            list->add(value); // remember to check if user wants to add before or after
            lastOperation = "Added node with value: " + inputString;
            inputString = "";
            mode = MODE_NONE;
        } else if (mode == MODE_UPDATE && nodeIndex != -1 && !inputString.empty()) {
            int value = std::stoi(inputString);
            Node* current = list->getHead();
            for (int i = 0; i < nodeIndex; i++) current = current -> next;
            Operation op(Operation::UPDATE, nodeIndex, current->val, value);
            operationHistory.push_back(op);
            list->update(nodeIndex, value);
            lastOperation = "Updated node at index " + to_string(nodeIndex) + 
                          " to value: " + inputString;
            inputString = "";
            mode = MODE_NONE;
            nodeIndex = -1;
        }
    }
}

void LinkedListVisualizer::updateAnimation() {
    if (!operationHistory.empty() && currentStep < operationHistory.size()) {
        animationProgress += GetFrameTime() * animationSpeed;
        if (animationProgress >= 1.0f) {
            animationProgress = 0.0f;
            if (currentStep < operationHistory.size() - 1) {
                currentStep++;
            }
        }
    }
}

void LinkedListVisualizer::stepForward() {
    if (currentStep < operationHistory.size() - 1) {
        currentStep++;
        animationProgress = 0.0f;
        applyOperation(operationHistory[currentStep]);
    }
}

void LinkedListVisualizer::stepBackward() {
    if (currentStep > 0) {
        undoOperation(operationHistory[currentStep]);
        currentStep--;
        animationProgress = 0.0f;
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
            list->insertAt(op.nodeIndex, op.oldValue);
            break;
        case Operation::UPDATE:
            list->update(op.nodeIndex, op.oldValue);
            break;
        case Operation::SEARCH:
            // Handle search undo
            break;
    }
}

void LinkedListVisualizer::applyAnimationEffects(float posX, float posY, Node* node, int index) {
    const Operation& currentOp = operationHistory[currentStep];
    switch (currentOp.type) {
        case Operation::ADD:
            posY += sin(animationProgress * PI * 2) * 10;
            break;
        case Operation::DELETE:
            float alpha = 1.0f - animationProgress;
            DrawCircle(posX, posY, 30.f, ColorAlpha(WHITE, alpha));
            DrawText(TextFormat("%d", node->val), posX - 10, posY - 10, 20, ColorAlpha(BLACK, alpha));
            break;
    }
}

bool LinkedListVisualizer::DrawButton(float x, float y, float width, float height, const char* text) {
    Rectangle btnRect = { x, y, width, height };
    bool clicked = false;
    
    if (CheckCollisionPointRec(GetMousePosition(), btnRect)) {
        DrawRectangleRec(btnRect, LIGHTGRAY);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            clicked = true;
        }
    } else {
        DrawRectangleRec(btnRect, GRAY);
    }
    
    float textWidth = MeasureText(text, 20);
    float textX = x + (width - textWidth) / 2;
    float textY = y + (height - 20) / 2;
    DrawText(text, textX, textY, 20, BLACK);
    
    return clicked;
}

float LinkedListVisualizer::GuiSlider(Rectangle bounds, const char* textLeft, const char* textRight, 
    float value, float minValue, float maxValue) {
    // Draw slider background
    DrawRectangleRec(bounds, LIGHTGRAY);

    // Calculate slider position based on current value
    float range = maxValue - minValue;
    float sliderPos = bounds.x + (bounds.width * ((value - minValue) / range));

    // Draw slider knob
    DrawCircle(sliderPos, bounds.y + bounds.height/2, 10, DARKGRAY);

    // Draw min/max labels
    DrawText(textLeft, bounds.x - MeasureText(textLeft, 16) - 5, 
    bounds.y + bounds.height/2 - 8, 16, DARKGRAY);
    DrawText(textRight, bounds.x + bounds.width + 5, 
    bounds.y + bounds.height/2 - 8, 16, DARKGRAY);

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