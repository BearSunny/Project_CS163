#pragma once

#include <string>
#include "raylib.h"

class InputField {
private:
    Rectangle box;              // Display area of the input field
    std::string inputText;      // Text entered by the user
    bool isActive;              // Indicates whether input is enabled
    int cursorPos;              // Current position of the cursor
    float blinkTimer;           // Timer for cursor blinking
    bool showCursor;            // Whether to show the caret
    float blinkInterval;        // Blinking speed (in seconds)
    float scrollOffset;         // Scroll offset for long text

    void UpdateScrollOffset();  // Update scroll offset internally

public:
    // Constructor
    InputField(float x, float y, float width, float height);

    // Methods
    void handleInput();
    void draw();
    void update(float deltaTime);
    std::string getText() const;
    void clearText();
    bool IsActive();
    void setActive(bool state);
    void updateCursorPosition(int mouseX);
};