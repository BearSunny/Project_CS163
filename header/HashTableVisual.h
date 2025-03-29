#pragma once

#include "Program.h"
#include "Items/Variables.h"
#include "Items/Button.h"
#include "Items/InputField.h"
#include "DataStructures/HashTable.h"

#include <iostream>
#include <string>
#include <cmath>

// Lớp HashTablePage
class HashTablePage : public Page {
private:
    Program* program;
    InputField inputField; // Commonly use for all features
    HashTable* table;
    float startX, startY, offsetX; // Vị trí vẽ các square
    int highlightedIdx; // chỉ số được highlight khi search

    BackButton backButton;
    Button createButton;  // Initialze the hash table by the size provided by user
    Button insertButton;
    Button deleteButton;
    Button searchButton;
    Button clearButton;
    Button newButton;

    bool tableCreated;  // Trạng thái đã tạo bảng hay chưa
public:
    HashTablePage(Program* prog);

    void handleInput() override;

    void update(float deltaTime) override;

    void draw() override;
};