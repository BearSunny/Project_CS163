#pragma once

// #include "Program.h"
#include "declare.h"
#include "HashButton.h"
#include "InputField.h"
#include "HashTable.h"

#include <iostream>
#include <string>
#include <cmath>

// Lớp HashTablePage
class HashTablePage {
private:
    // Program* program;
    InputField inputField; // Commonly use for all features
    HashTable* table;
    float startX, startY, offsetX; // Vị trí vẽ các square
    int highlightedIdx; // chỉ số được highlight khi search

    // BackHashButton backHashButton;
    HashButton createButton;  // Initialze the hash table by the size provided by user
    HashButton insertButton;
    HashButton deleteButton;
    HashButton searchButton;
    HashButton clearButton;
    HashButton newButton;

    bool tableCreated;  // Trạng thái đã tạo bảng hay chưa

public:
    HashTablePage();

    void handleInput();

    void update(float deltaTime);

    void draw();
};