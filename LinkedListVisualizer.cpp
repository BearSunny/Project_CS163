#include "LinkedListVisualizer.h"
#include <sstream>
#include <raylib.h>

LinkedListVisualizer::LinkedListVisualizer(DummyLinkedList* list)
    : list(list), mode(MODE_NONE), inputString(""), nodeIndex(-1) {}

void LinkedListVisualizer::draw() {
    float startX = 50.f;
    float startY = GetScreenHeight() / 2.f;
    float offsetX = 100.f;  // Horizontal spacing between nodes
    Node* current = list->getHead();
    int index = 0;

    while (current != nullptr) {
        float posX = startX + index * offsetX;
        float posY = startY;

        DrawCircle(posX, posY, 30.f, WHITE);

        // Set node value as text
        DrawText(TextFormat("%d", current->val), posX - 10, posY - 10, 20, BLACK);

        // Highlight when search is successful
        if (index == nodeIndex ||
            (mode == MODE_SEARCH && !inputString.empty() && std::to_string(current->val) == inputString)) {
            DrawCircle(posX, posY, 35.f, BLUE);
        }

        // Draw arrow to next node if exists
        if (current->next != nullptr) {
            DrawLine(posX + 30, posY, posX + offsetX - 30, posY, BLACK);
            DrawTriangle((Vector2){posX + offsetX - 30, posY}, (Vector2){posX + offsetX - 40, posY - 5}, (Vector2){posX + offsetX - 40, posY + 5}, BLACK);
        }

        current = current->next;
        index++;
    }

    DrawRectangle(50, 50, 300, 50, DARKGRAY);
    DrawText(inputString.c_str(), 60, 60, 18, WHITE);
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
            list->add(value); // remember to check if user wants to add before or after
            inputString = "";
            mode = MODE_NONE;
        } else if (mode == MODE_UPDATE && nodeIndex != -1 && !inputString.empty()) {
            int value = std::stoi(inputString);
            list->update(nodeIndex, value);
            inputString = "";
            mode = MODE_NONE;
            nodeIndex = -1;
        }
    }
}