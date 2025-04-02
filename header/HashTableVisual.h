#pragma once

#include "declare.h"
#include "HashButton.h"
#include "InputField.h"
#include "HashTable.h"
#include "../source/HashButton.cpp"
#include "../source/InputField.cpp"
#include "../source/HashTable.cpp"

#include <iostream>
#include <string>
#include <cmath>

// Lớp HashTablePage
class HashTablePage {
private:
    InputField inputField; // Commonly use for all features
    InputField filePathInput;
    HashTable* table;
    float startX, startY, offsetX; // Vị trí vẽ các square
    int highlightedIdx; // chỉ số được highlight khi search

    HashButton createButton;  // Initialze the hash table by the size provided by user
    HashButton insertButton;
    HashButton deleteButton;
    HashButton searchButton;
    HashButton clearButton;
    HashButton newButton;
    HashButton loadFileButton;
    HashButton randomButton;
    BackHashButton closeLoadFileButton;

    bool tableCreated;  // Trạng thái đã tạo bảng hay chưa

public:
    HashTablePage();
    ~HashTablePage();

    void handleInput();

    void update(float deltaTime);

    void draw();
};