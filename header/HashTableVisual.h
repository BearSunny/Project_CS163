#pragma once

#include "declare.h"
#include "HashButton.h"
#include "InputField.h"
#include "HashTable.h"
#include "Step.h"
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <raymath.h>

// Forward declaration of DisplayHash function
void DisplayHash();

// Class definition for HashTablePage
class HashTablePage {
private:
    // Input fields
    InputField inputField; // Commonly used for all features
    InputField filePathInput;

    // Hash table and drawing properties
    HashTable* table;
    float startX, startY, offsetX; // Drawing positions
    int cellSize;
    int highlightedIdx; // Index to be highlighted when searching

    // Buttons for various actions
    HashButton createButton;
    HashButton insertButton;
    HashButton deleteButton;
    HashButton searchButton;
    HashButton clearButton;
    HashButton newButton;
    HashButton loadFileButton;
    HashButton randomButton;
    BackHashButton closeLoadFileButton;
    CircleSlideButton speedSlider;

    HashButton stepModeButton;       // Display-only button
    HashButton OnOffButton;          // Toggle Step Mode
    HashButton browseButton;         // Browse File button
    HashButton playButton;

    // Table and step-by-step mode states
    bool tableCreated;               // Whether the table has been created
    bool stepModeOn;                 // Step-by-Step Mode toggle
    bool insertStepModeOn;           // Insert Step-by-Step toggle
    bool deleteStepModeOn;           // Delete Step-by-Step toggle
    bool searchStepModeOn;           // Search Step-by-Step toggle

    // Step management
    std::vector<Step> steps;         // List of steps
    int currentStep;                 // Current step
    int pendingInsertKey;            // Key for pending step-by-step insert

    // Animation properties
    bool isPlaying = false;         // Trạng thái Play/Pause
    float animationProgress = 0.0f; // Tiến độ animation (0.0 -> 1.0)
    float playSpeed = 1.0f;         // Tốc độ chuyển động (có thể điều chỉnh)
    float progressValue = 0.0f;     // Giá trị progress từ 0.0 đến 1.0
    float playSpeedMax = 4.0f;

    // Source code display for operations
    std::vector<std::string> codeLinesForInsert;
    std::vector<std::string> codeLinesForDelete;
    std::vector<std::string> codeLinesForSearch;

public:
    // Constructor and Destructor
    HashTablePage();
    ~HashTablePage();

    // Public methods
    void handleInput();
    void update(float deltaTime);
    void draw();
    void buildInsertSteps(int key);
    void buildDeleteSteps(int key);
    void buildSearchSteps(int key);
};