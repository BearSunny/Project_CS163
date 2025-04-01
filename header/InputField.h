#pragma once

#include <string>
#include "raylib.h"

class InputField
{
private:
    Rectangle box;        // Vùng hiển thị ô nhập
    std::string inputText; 
    bool isActive;        // Cho phép nhập hay không

public:
    InputField(float x, float y, float width, float height);

    void handleInput();
    
    void draw();

    std::string getText() const { 
        return inputText; 
    }

    void clearText() { 
        inputText.clear(); 
    }

    bool IsActive();
    void setActive(bool state);
};