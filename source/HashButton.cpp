#include "../header/HashButton.h"

HashButton::HashButton(float x, float y, float width, float height, const std::string& str, int sizeFont) {
    shape = { x, y, width, height };
    text = str;
    idleColor = Color {248, 186, 200, 255};
    hoverColor = Color {245, 162, 178, 255};
    hovered = false;
    borderColor = Color {194, 24, 91, 255};
    fontSize = sizeFont;
    textColor = Color {255, 254, 206, 255};
}

void HashButton::handleInput() {
    Vector2 mousePos = GetMousePosition();
    hovered = CheckCollisionPointRec(mousePos, shape);

    // Nếu hovered, ta có thể đổi màu v.v... (vẽ trong hàm draw)
}

bool HashButton::isClicked() {
    if (hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        return true;
    }
    return false;
}

void HashButton::draw() {
    // Đổi màu nếu hovered
    Color currentColor = hovered ? hoverColor : idleColor;
    DrawRectangleRounded(shape, 0.4f, 10, currentColor);
    DrawRectangleRoundedLines(shape, 0.4f, 10, borderColor);

    // Tính toán để vẽ text chính giữa HashButton
    Vector2 textSize = MeasureTextEx(FONT, text.c_str(), fontSize, 1);
    float textX = shape.x + (shape.width - textSize.x) / 2.0f;
    float textY = shape.y + (shape.height - textSize.y) / 2.0f;

    DrawTextEx(FONT, text.c_str(), {textX, textY}, fontSize, 1, textColor);
}

void BackHashButton::draw() {
    // Đổi màu nếu hovered
    Color currentColor = hovered ? hoverColor : idleColor;
    DrawRectangle(shape.x, shape.y, shape.width, shape.height, currentColor);
    DrawRectangleLinesEx(shape, 2, BLACK);

    // Tính toán để vẽ text chính giữa HashButton
    Vector2 textSize = MeasureTextEx(FONT, text.c_str(), fontSize, 1);
    float textX = shape.x + (shape.width - textSize.x) / 2.0f;
    float textY = shape.y + (shape.height - textSize.y) / 2.0f;

    DrawTextEx(FONT, text.c_str(), {textX, textY}, fontSize, 1, BLACK);
}
