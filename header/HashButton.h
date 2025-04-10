#pragma once

#include "declare.h"
#include <string>
#include <raylib.h>

class HashButton {
protected:
    Rectangle shape;      // Hình chữ nhật của nút
    std::string text;     // Nội dung nút
    Color idleColor;      // Màu khi không hover
    Color hoverColor;     // Màu khi hover
    Color borderColor;    // Màu viền
    bool hovered;         // Trạng thái hover
    int fontSize;
    Color textColor;

public:
    HashButton(float x, float y, float width, float height, 
           const std::string& str, int sizeFont);

    void handleInput();
    bool isClicked(); // Trả về true nếu click
    virtual void draw();

    void setIdleColor(Color color) { idleColor = color; }
    void setHoverColor(Color color) { hoverColor = color; }
    void setBorderColor(Color color) { borderColor = color; }
    void setTextColor(Color color) { textColor = color; }
    void setText(std::string textSet) { text = textSet; }

    // Thêm các phương thức getter
    float getX() const { return shape.x; }
    float getY() const { return shape.y; }
    float getWidth() const { return shape.width; }
    float getHeight() const { return shape.height; }
};

class BackHashButton : public HashButton {
public: 
    BackHashButton(float x, float y, float width, float height, const std::string& str, int sizeFont): 
        HashButton(x, y, width, height, str, sizeFont){}

    void draw() override;
};

class CircleSlideButton {
private:
    float x;
    float y;
    float radius;
    bool hovered;

public:
    CircleSlideButton(float x, float y, float radius);

    void handleInput();
    bool isHold();
    virtual void draw();

    float getX() { return x; }
    void setX(float x) { this->x = x; }
};