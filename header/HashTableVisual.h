#pragma once

#include "declare.h"
#include "HashButton.h"
#include "InputField.h"
#include "HashTable.h"
#include "Step.h"
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

    HashButton stepModeButton;        // Nút hiển thị, không có thao tác gì
    HashButton OnOffButton;           // Nút bật/tắt Step Mode  
    HashButton browseButton;          // Nút Browse File

    bool tableCreated;  // Trạng thái đã tạo bảng hay chưa
    bool stepModeOn;              // Nút Run Step by Step đang On hay Off
    bool insertStepModeOn;        // Chế độ Insert Step by Step đang On hay Off
    bool deleteStepModeOn;        // Chế độ Delete Step by Step đang On hay Off
    bool searchStepModeOn;        // Chế độ Search Step by Step đang On hay Off
    std::vector<Step> steps;      // Danh sách các bước
    int currentStep;              // Bước hiện tại
    int pendingInsertKey;         // Lưu key cần thao tác Step by Step
    bool isLastStep;              // Trạng thái có phải Step cuối hay chưa

    std::vector<std::string> codeLinesForInsert;  // Source code hiển thị cho Insert
    std::vector<std::string> codeLinesForDelete;  // Source code hiển thị cho Delete
    std::vector<std::string> codeLinesForSearch;  // Source code hiển thị cho Search

public:
    HashTablePage();
    ~HashTablePage();

    void handleInput();

    void update(float deltaTime);

    void draw();

    void buildInsertSteps(int key); // Hàm xây dựng các step khi Search
    void buildDeleteSteps(int key); // Hàm xây dựng các step khi Search
    void buildSearchSteps(int key); // Hàm xây dựng các step khi Search
};