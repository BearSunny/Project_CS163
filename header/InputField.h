#pragma once

#include <string>
#include "raylib.h"

class InputField
{
private:
    Rectangle box;        // Vùng hiển thị ô nhập
    std::string inputText; 
    bool isActive;        // Cho phép nhập hay không
    int cursorPos = 0;              // Vị trí con trỏ
    float blinkTimer = 0.0f;        // Bộ đếm thời gian nhấp nháy
    bool showCursor = true;         // Trạng thái hiển thị caret
    float blinkInterval = 0.5f;     // Tốc độ nhấp nháy (giây)
    float scrollOffset = 0.0f;      // Độ dịch chuyển khi text dài

public:
    InputField(float x, float y, float width, float height);

    void handleInput();
    void draw();
    void update(float deltaTime);

    std::string getText() const { 
        return inputText; 
    }

    void clearText() { 
        inputText.clear(); 
    }

    bool IsActive();
    void setActive(bool state);

    void updateCursorPosition(int mouseX);
    void UpdateScrollOffset();
};