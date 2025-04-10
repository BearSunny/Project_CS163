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

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, box)) {
            // Cập nhật vị trí con trỏ dựa trên vị trí chuột
            updateCursorPosition(mousePos.x);
            blinkTimer = 0.0f;
            showCursor = true;
            UpdateScrollOffset();
        }
    }
    if (IsKeyPressed(KEY_LEFT)) {
        cursorPos = std::max(0, cursorPos - 1);
        blinkTimer = 0.0f;
        UpdateScrollOffset();
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        cursorPos = std::min((int)inputText.length(), cursorPos + 1);
        blinkTimer = 0.0f;
        UpdateScrollOffset();
    }
    if (IsKeyPressed(KEY_HOME)) {
        cursorPos = 0;
        UpdateScrollOffset();
    }
    if (IsKeyPressed(KEY_END)) {
        cursorPos = inputText.length();
        UpdateScrollOffset();
    }

    // Lấy phím ký tự bằng hàm GetCharPressed()
    int key = GetCharPressed();
    while (key > 0)
    {
        // Ký tự ASCII in được (từ 32 (space) đến 126 (~))
        if (key >= 32 && key <= 126)
        {
            // Đảm bảo cursorPos hợp lệ trước khi chèn ký tự
            if (cursorPos > (int)inputText.size()) {
                cursorPos = inputText.size();
            }
            inputText.insert(cursorPos, 1, (char)key);
            cursorPos++;
            blinkTimer = 0.0f;
            UpdateScrollOffset();
        }
        // Lấy ký tự tiếp
        key = GetCharPressed();
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        if (cursorPos > 0 && !inputText.empty())
        {
            inputText.erase(cursorPos - 1, 1);
            cursorPos--;
            blinkTimer = 0.0f;
            UpdateScrollOffset();
        }
    }

    // Xử lý paste (Ctrl+V)
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
        const char* clipboard = GetClipboardText();
        if (clipboard) {
            std::string pasteText = clipboard;
            // Chèn toàn bộ nội dung paste vào vị trí con trỏ
            inputText.insert(cursorPos, pasteText);
            cursorPos += pasteText.length();
            blinkTimer = 0.0f;
            UpdateScrollOffset();
        }
    }
}

void InputField::draw() {
    // Vẽ khung
    DrawRectangleRec(box, WHITE);
    DrawRectangleLinesEx(box, 2, BLACK);

    int fontSize = 25;
    // Bắt đầu chế độ cắt vẽ (scissor mode) để chỉ vẽ trong vùng box
    BeginScissorMode(box.x, box.y, box.width, box.height);

    // Vẽ text. Ta đặt vị trí x có một margin (10px) và trừ đi scrollOffset để hiển thị phần cuộn
    Vector2 textPos = { box.x + 10 - scrollOffset, box.y + (box.height - fontSize) / 2.0f };
    DrawTextEx(FONT, inputText.c_str(), textPos, fontSize, 1, BLACK);

    EndScissorMode();

    // Vẽ caret nếu input field đang active
    if (isActive && showCursor) {
        // Tính vị trí caret dựa trên đoạn text từ đầu đến vị trí cursor
        std::string textBeforeCursor = inputText.substr(0, cursorPos);
        float caretOffset = MeasureTextEx(FONT, textBeforeCursor.c_str(), fontSize, 1).x;
        Vector2 caretPos = { box.x + 10 + caretOffset - scrollOffset, box.y + (box.height - fontSize) / 2.0f };
        DrawLineEx(caretPos, { caretPos.x, caretPos.y + fontSize }, 2, BLACK);
    }
}

void InputField::update(float deltaTime) {
    if (isActive) {
        blinkTimer += deltaTime;
        if (blinkTimer >= blinkInterval) {
            showCursor = !showCursor;
            blinkTimer = 0.0f;
        }
    } else {
        showCursor = false;
    }
}

void InputField::setActive(bool state) {
    isActive = state;
    if (isActive) {
        cursorPos = inputText.length();
        blinkTimer = 0.0f;
        showCursor = true;
        UpdateScrollOffset();
    }
}

bool InputField::IsActive() {
    return isActive;
}

void InputField::updateCursorPosition(int mouseX) {
    int fontSize = 25;
    float startX = box.x + 10;
    float currentX = startX;
    cursorPos = 0;

    // Duyệt qua từng ký tự để xác định vị trí con trỏ dựa trên vị trí chuột
    for (size_t i = 0; i < inputText.size(); ++i) {
        // Sử dụng cùng fontSize với draw()
        float charWidth = MeasureTextEx(FONT, &inputText[i], fontSize, 1).x;
        if (mouseX < currentX + charWidth / 2) {
            break;
        }
        currentX += charWidth;
        cursorPos++;
    }
}

void InputField::UpdateScrollOffset() {
    int fontSize = 25;
    float visibleWidth = box.width - 20; // margin trái phải (10 px mỗi bên)
    // Tính vị trí caret tính từ đầu chuỗi
    float caretX = MeasureTextEx(FONT, inputText.substr(0, cursorPos).c_str(), fontSize, 1).x;

    // Nếu caret vượt quá vùng hiển thị bên phải thì cập nhật scrollOffset
    if (caretX - scrollOffset > visibleWidth) {
        scrollOffset = caretX - visibleWidth;
    }
    // Nếu caret di chuyển về bên trái quá vùng hiển thị, giảm scrollOffset
    else if (caretX - scrollOffset < 0) {
        scrollOffset = caretX;
    }
}