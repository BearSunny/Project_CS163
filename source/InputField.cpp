#include "../header/InputField.h"
#include "raylib.h"
#include <algorithm>

InputField::InputField(float x, float y, float width, float height) {
    box = { x, y, width, height };
    inputText = "";
    isActive = true; // Mặc định cho phép nhập
}

void InputField::handleInput() {
    if (!isActive) return;

    // Lấy phím ký tự bằng hàm GetCharPressed()
    int key = GetCharPressed();
    while (key > 0)
    {
        // Ký tự ASCII in được (từ 32 (space) đến 126 (~))
        if ((key >= 32) && (key <= 126))
        {
            inputText.push_back((char)key);
        }
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (!inputText.empty())
        {
            inputText.pop_back();
        }
    }
}

void InputField::draw() {
    // Vẽ khung
    DrawRectangleRec(box, WHITE);
    DrawRectangleLinesEx(box, 2, BLACK);

    // Vẽ text
    int fontSize = 20;
    Vector2 pos = { box.x + 5, box.y + (box.height - fontSize) / 2.0f };
    DrawTextEx(GetFontDefault(), inputText.c_str(), pos, fontSize, 1, BLACK);
}

void InputField::setActive(bool state) {
    isActive = state;
}

bool InputField::IsActive() {
    return isActive;
}