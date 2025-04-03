#include "../header/HashTableVisual.h"

HashTablePage::HashTablePage() : 
    inputField((screenWidth - 400)/2, 100, 400, 50),
    filePathInput((screenWidth - 400)/2, (screenHeight - 50)/2, 400, 50),
    table(nullptr),
    startX(450), startY(170), offsetX(30),
    highlightedIdx(-1),

    createButton(10, 170, 250, 40, "Create Table", 30), // Modidy the coordinate after
    insertButton(10, 220, 250, 40, "Insert", 30),
    deleteButton(10, 270, 250, 40, "Delete", 30),
    searchButton(10, 320, 250, 40, "Search", 30),
    clearButton(10, 370, 250, 40, "Clear", 30),
    newButton(10, 420, 250, 40, "New", 30),
    loadFileButton(10, 470, 250, 40, "Load File", 30),
    randomButton(10, 520, 250, 40, "Random", 30),
    closeLoadFileButton((screenWidth - 400)/2.0 + 400, (screenHeight - 50)/2.0, 100, 50, "Close", 30),

    stepModeButton(10, 570, 250, 40, "Run Step by Step", 30),
    OnOffButton(270, 570, 100, 40, "Off", 30),
    browseButton(270, 470, 100, 40, "Browse", 30),

    tableCreated(false),
    stepModeOn(false),
    insertStepModeOn(false),
    deleteStepModeOn(false),
    searchStepModeOn(false),
    currentStep(0)
{ 
    filePathInput.setActive(false);

    codeLinesForInsert = {
        "int idx = -1;",
        "if (search(key, idx))", 
        "   return;",
        "idx = key % TABLE_SIZE;",
        "while (occ[idx])",
        "   idx = (idx + 1) % TABLE_SIZE;",
        "table[idx] = key;",
        "occ[idx] = true;"
    };

    codeLinesForDelete = {
        "int idx = key % TABLE_SIZE;",
        "while (occ[idx] && table[idx] != key) {",
        "    idx = (idx + 1) % TABLE_SIZE;",
        "}",
        "if (occ[idx] && table[idx] == key) {",
        "    occ[idx] = false;",
        "    return true;",
        "}",
        "return false;"
    };

    codeLinesForSearch = {
        "int cnt = 0;",
        "int idx = key % TABLE_SIZE;",
        "while (occ[idx] && table[idx] != key) {",
        "    idx = (idx + 1) % TABLE_SIZE;",
        "    cnt++;",
        "    if (cnt == TABLE_SIZE) return false;",
        "}",
        "if (occ[idx] && table[idx] == key) {",
        "    return true;",
        "}",
        "return false;"
    };
}

HashTablePage::~HashTablePage() {
    delete table;
    table = nullptr;
}

void HashTablePage::handleInput() 
{
    randomButton.handleInput();
    if (randomButton.isClicked())
    {
        std::srand((unsigned)std::time(nullptr));
        int randomSize = std::rand() % 61;
        if (table != nullptr) {
            delete table;
            table = new HashTable(randomSize);
        } else {
            table = new HashTable(randomSize);
        }
        // Use the current time as the seed for the random number generator
        int randomNum = std::rand() % (randomSize + 1);
        for (int i = 0; i < randomNum; i++) {
            int key = std::rand() % 100000 ;
            table->insert(key);
        }
        highlightedIdx = -1;
        tableCreated = true;
        filePathInput.setActive(false);
        inputField.setActive(true);
    } 

    // Chỉ xử lý 1 trong 2 trước
    if (filePathInput.IsActive()) { 
        filePathInput.handleInput();

        FilePathList droppedFiles = LoadDroppedFiles(); // Lấy danh sách file được kéo thả
        if (droppedFiles.count > 0) {
            std::string path = std::string(droppedFiles.paths[0]);
            UnloadDroppedFiles(droppedFiles); // Giải phóng bộ nhớ danh sách file
            if (table == nullptr) { // Tránh gây crash khi gọi loadHashTableFromFile()
                table = new HashTable(1); // Size tạm, sẽ được cập nhật khi load file
            }
            if (table->loadHashTableFromFile(path)) {
                tableCreated = true;
                highlightedIdx = -1;
                filePathInput.clearText();
                // filePathInput.setActive(false);
                // inputField.setActive(true);
            }
            else {
                TraceLog(LOG_WARNING, "Load file failed: %s", path.c_str());
            }
            filePathInput.setActive(false);
            inputField.setActive(true);
        }

        if (IsKeyPressed(KEY_ENTER)) {
            std::string path = filePathInput.getText();
            if (!path.empty())
            {
                if (table == nullptr) { // Tránh gây crash khi gọi loadHashTableFromFile()
                    table = new HashTable(1); // Size tạm, sẽ được cập nhật khi load file
                }
                if (table->loadHashTableFromFile(path)) {
                    tableCreated = true;
                    highlightedIdx = -1;
                    filePathInput.clearText();
                    // filePathInput.setActive(false);
                    // inputField.setActive(true);
                }
                else {
                    TraceLog(LOG_WARNING, "Load file failed: %s", path.c_str());
                }
                filePathInput.setActive(false);
                inputField.setActive(true);
            }
            // filePathInput.clearText();
        }
    }
    else {
        browseButton.handleInput();
        inputField.handleInput();
    }

    if (!tableCreated) { // Chưa tạo bảng, chỉ xử lý nút Create Table và Load File
        createButton.handleInput();
        loadFileButton.handleInput();

        if (createButton.isClicked() || IsKeyPressed(KEY_ENTER))
        {
            std::string str = inputField.getText();
            if (!str.empty())
            {
                try
                {
                    int size = std::stoi(str);
                    if(size > 0)
                    {
                        table = new HashTable(size);
                        tableCreated = true;
                        highlightedIdx = -1;
                    }
                }
                catch (...) {
                    // Bỏ qua nếu lỗi chuyển đổi
                }
            }
            inputField.clearText();
        }

        if (loadFileButton.isClicked())
        {
            filePathInput.setActive(true);
            inputField.setActive(false);
        }

        closeLoadFileButton.handleInput();
        if (closeLoadFileButton.isClicked()) {
            filePathInput.setActive(false);
            inputField.setActive(true);
        }

        return;
    }

    // Should be handled after creation
    insertButton.handleInput(); 
    deleteButton.handleInput();
    searchButton.handleInput();
    clearButton.handleInput();
    newButton.handleInput();
    OnOffButton.handleInput();

    if (OnOffButton.isClicked()) {
        stepModeOn = !stepModeOn;
        if (stepModeOn) {
            OnOffButton.setText("On");
        } else {
            OnOffButton.setText("Off");
        }
    }

    if (stepModeOn) {
        // Nhấn Next => currentStep++
        if (IsKeyPressed(KEY_RIGHT)) {
            if (insertStepModeOn) {
                if (currentStep < (int)steps.size() - 1) {
                    currentStep++;
                    if (currentStep == (int)steps.size() - 2) 
                        table->insert(pendingInsertKey);
                } 
            } 
            if (deleteStepModeOn) {
                if (currentStep < (int)steps.size() - 1) {
                    currentStep++;
                    if (currentStep == (int)steps.size() - 2) {
                        int idx = -1;
                        table->remove(pendingInsertKey, idx);
                    }
                } 
            } 
            if (searchStepModeOn) {
                if (currentStep < (int)steps.size() - 1) {
                    currentStep++;
                } 
            }
        }
        // Nhấn Prev => currentStep--
        if (IsKeyPressed(KEY_LEFT)) {
            if (insertStepModeOn) {
                if (currentStep == (int)steps.size() - 2) { // Key đã được insert
                    int idx = -1;
                    table->remove(pendingInsertKey, idx);
                }
            }
            if (deleteStepModeOn) {
                if (currentStep == (int)steps.size() - 2) { // Key đã được delete
                    table->insert(pendingInsertKey);
                }
            } 
            if (searchStepModeOn) {
                
            }

            if (currentStep > 0) {
                currentStep--;
            }
        }
    }

    // Should be handled after creation
    insertButton.handleInput(); 
    deleteButton.handleInput();
    searchButton.handleInput();
    clearButton.handleInput();
    newButton.handleInput();

    if (insertButton.isClicked() || IsKeyPressed(KEY_ENTER))
    {
        std::string str = inputField.getText();
        if (!str.empty())
        {
            try
            {
                int key = std::stoi(str);
                if (!stepModeOn) {
                    table->insert(key);
                    highlightedIdx = -1; // reset highlight
                } else {
                    buildInsertSteps(key);
                    insertStepModeOn = true;
                    deleteStepModeOn = searchStepModeOn = false;
                }
            }
            catch (...)
            {
                // Nếu không chuyển được, bỏ qua
            }
            // Xoá nội dung ô nhập
            inputField.clearText();
        }
    }

    if (deleteButton.isClicked())
    {
        std::string str = inputField.getText();
        if (!str.empty())
        {
            try
            {
                int key = std::stoi(str);
                if (!stepModeOn) {
                    int idx;
                    if (table->remove(key, idx)) {
                        // Nếu xóa thành công, có thể highlight ô vừa xóa (màu khác) trong một khoảng thời gian ngắn
                        highlightedIdx = idx;
                    } else {
                        highlightedIdx = -1;
                    }
                } else {
                    buildDeleteSteps(key);
                    deleteStepModeOn = true;
                    insertStepModeOn = searchStepModeOn = false;
                }
            }
            catch (...)
            {
                // Bỏ qua nếu lỗi chuyển đổi
            }
            inputField.clearText();
        }
    }

    if (searchButton.isClicked())
    {
        std::string str = inputField.getText();
        if (!str.empty())
        {
            try
            {
                int key = std::stoi(str);
                if (!stepModeOn) {
                    int idx = -1;
                    if (table->search(key, idx)) highlightedIdx = idx;
                    else highlightedIdx = -1;
                } else {
                    buildSearchSteps(key);
                    searchStepModeOn = true;
                    insertStepModeOn = deleteStepModeOn = false;
                }
            }
            catch (...)
            {
                // Bỏ qua nếu lỗi
            }
            inputField.clearText();
        }
    }

    if (clearButton.isClicked())
    {
        if (table != nullptr)
        {
            int size = table->getTableSize();
            delete table;
            table = new HashTable(size);
            highlightedIdx = -1;
        }
    }

    if (newButton.isClicked())
    {
        if (table != nullptr)
        {
            delete table;
            table = nullptr;
            highlightedIdx = -1;
            tableCreated = false;
        }
    }

    if (browseButton.isClicked()) {

    }
}

void HashTablePage::update(float deltaTime) 
{
    inputField.update(deltaTime);
    filePathInput.update(deltaTime);
}

void HashTablePage::draw() 
{
    // BeginDrawing();
    ClearBackground((Color){241, 231, 231, 255});
    createButton.draw();
    insertButton.draw();
    deleteButton.draw();
    searchButton.draw();
    clearButton.draw();
    newButton.draw();
    loadFileButton.draw();
    randomButton.draw();
    inputField.draw();
    stepModeButton.draw();
    OnOffButton.draw();
    browseButton.draw();

    if (filePathInput.IsActive()) {
        const char *text = "Drag and drop input file or type the file path into the below box";
        Vector2 textSize = MeasureTextEx(FONT, text, 35, 1);
        DrawTextEx(FONT, text, Vector2{(screenWidth - textSize.x)/2, 375}, 35, 1, Color{87, 143, 202, 255});
        filePathInput.draw();
        closeLoadFileButton.draw();
    }

    // Vẽ tiêu đề và hướng dẫn
    if (!tableCreated)
    {
        // Khi chưa tạo bảng, yêu cầu người dùng nhập kích thước của bảng
        const char *text = "Enter table size first (Should not be exceed 60)";
        Vector2 textSize = MeasureTextEx(FONT, text, 35, 1);
        DrawTextEx(FONT, text, Vector2{(screenWidth - textSize.x)/2, 50}, 35, 1, Color{87, 143, 202, 255});
    }
    else
    {
        const char *text = "Insert values into the below box";
        Vector2 textSize = MeasureTextEx(FONT, text, 35, 1);
        DrawTextEx(FONT, text, Vector2{(screenWidth - textSize.x)/2, 50}, 35, 1, Color{87, 143, 202, 255});
    }

    // Nếu bảng đã được tạo, biểu diễn HashTable dưới dạng các ô vuông
    if (tableCreated && table != nullptr)
    {
        if (stepModeOn && currentStep >= 0 && currentStep < (int)steps.size()) {
            highlightedIdx = steps[currentStep].highlightedIndex;
        }

        int tableSize = table->getTableSize();
        const int cellSize = 80;  
        for (int i = 0; i < tableSize; i++) // Traverse the whole table
        {
            float cellX = startX + (i % 10 + 0.5)*(cellSize + offsetX);
            float cellY = startY + (i / 10)*(cellSize + 40);

            Color cellColor = (i == highlightedIdx) ? Color {255, 254, 206, 255} : RAYWHITE;
            DrawRectangle(cellX, cellY, cellSize, cellSize, cellColor);
            DrawRectangleLines(cellX, cellY, cellSize, cellSize, BLACK);

            if (table->isOccupied(i))
            {
                std::string keyStr = std::to_string(table->getKeyAt(i));
                int fontSize = 25;
                Vector2 textSizeKey = MeasureTextEx(FONT, keyStr.c_str(), fontSize, 1);
                DrawTextEx(FONT, keyStr.c_str(), 
                           Vector2{ cellX + (cellSize - textSizeKey.x) / 2, cellY + (cellSize - textSizeKey.y) / 2 },
                           fontSize, 1, BLACK);
            }

            std::string idxStr = std::to_string(i);
            int idxFontSize = 20;
            Vector2 idxTextSize = MeasureTextEx(FONT, idxStr.c_str(), idxFontSize, 1);
            DrawTextEx(FONT, idxStr.c_str(), 
                       Vector2{ cellX + (cellSize - idxTextSize.x) / 2, cellY + cellSize + 5 },
                       idxFontSize, 1, BLACK);
        }
    }

    if (stepModeOn) {
        DrawRectangle(0, 620, 450, screenHeight - 620, Color {248, 186, 200, 255});
        DrawRectangleLines(0, 620, 450, screenHeight - 620, Color {194, 24, 91, 255});

        // Nếu stepModeOn == true, highlight dòng code = steps[currentStep].codeLine
        int highlightLine = -1;
        if (stepModeOn && currentStep >= 0 && currentStep < (int)steps.size()) {
            highlightLine = steps[currentStep].codeLine;
        }

        // Vẽ từng dòng code
        float lineHeight = 16.0f;
        std::vector<std::string> curCodeLines;
        if (insertStepModeOn)
            curCodeLines = codeLinesForInsert;
        if (deleteStepModeOn)
            curCodeLines = codeLinesForDelete;
        if (searchStepModeOn)
            curCodeLines = codeLinesForSearch;
        for (int i = 0; i < (int)curCodeLines.size(); i++) {
            float lineY = 620 + 10 + i * lineHeight;
            if (i == highlightLine) {
                DrawRectangle(0, lineY, 450, lineHeight, Color {245, 162, 178, 255});
            }
            DrawTextEx(FONT, curCodeLines[i].c_str(), {5, lineY}, 16, 1, BLACK);
        }

        // Nếu muốn vẽ mô tả step hiện tại
        if (stepModeOn && currentStep < (int)steps.size()) {
            DrawTextEx(FONT, steps[currentStep].description.c_str(), {5, screenHeight - 25}, 16, 1, RED);
        }
    }

    // EndDrawing();
}

void HashTablePage::buildInsertSteps(int key) {
    pendingInsertKey = key;
    steps.clear();
    currentStep = 0;

    int idx = key % table->getTableSize();
    Step s;

    s.codeLine = 0; 
    s.description = "Initialize a dummy idx to serve for search().";
    steps.push_back(s);

    // Bước 1: Checking the existence
    s.codeLine = 1; 
    s.description = "Checking the existence...";
    steps.push_back(s);
    int temp = -1;
    if (table->search(key, temp)) {
        s.highlightedIndex = idx;
        s.codeLine = 2;
        s.description = "The key is already existed in the table!";
        steps.push_back(s);
        return;
    }

    // Bước 2: Tính idx
    s.highlightedIndex = idx;
    s.codeLine = 3; 
    s.description = "Compute idx = key % TABLE_SIZE = " + std::to_string(key) + " % " + std::to_string(table->getTableSize());
    steps.push_back(s);

    // Bước 3+: Vòng while
    while (table->isOccupied(idx)) {
        // Mỗi vòng lặp => 1 bước
        s.highlightedIndex = idx;
        s.codeLine = 4;
        s.description = "Check condition of occ[idx]...";
        steps.push_back(s);

        // Bước 3b: idx = (idx + 1) % TABLE_SIZE;
        idx = (idx + 1) % table->getTableSize();
        s.highlightedIndex = idx;
        s.codeLine = 5; 
        s.description = "Collision, so idx++ (Linear Probing)!";
        steps.push_back(s);

        // // Bước 3c: cnt++
        // s.highlightedIndex = idx;
        // s.codeLine = 4;
        // s.description = "Increment cnt!";
        // steps.push_back(s);

        // // Bước 3d: if (cnt == TABLE_SIZE) return false;
        // if (cnt == table->getTableSize()) {
        //     s.highlightedIndex = -1;
        //     s.codeLine = 5;
        //     s.description = "The table is full, key was not founded!";
        //     steps.push_back(s);
        //     return; // Kết thúc buildSteps
        // }
    }

    s.highlightedIndex = idx;
    s.codeLine = 6;
    s.description = "Add key into the table!";
    steps.push_back(s);
    
    s.highlightedIndex = idx;
    s.codeLine = 7;
    s.description = "Set the occurence of key is true!";
    steps.push_back(s);
}

void HashTablePage::buildDeleteSteps(int key) {
    pendingInsertKey = key;
    steps.clear();
    currentStep = 0;

    int idx = key % table->getTableSize();
    Step s;
    
    // Bước 1: Tính idx
    s.highlightedIndex = idx;
    s.codeLine = 0; 
    s.description = "Compute idx = key % TABLE_SIZE = " + std::to_string(key) + " % " + std::to_string(table->getTableSize());
    steps.push_back(s);

    // Bước 2: Vòng lặp
    while (table->isOccupied(idx) && table->getKeyAt(idx) != key) {
        // Mỗi vòng lặp => 1 bước
        s.highlightedIndex = idx;
        s.codeLine = 1;
        s.description = "Check condition occ[idx] && table[idx] != key";
        steps.push_back(s);

        // Bước 3b: idx = (idx + 1) % TABLE_SIZE;
        idx = (idx + 1) % table->getTableSize();
        s.highlightedIndex = idx;
        s.codeLine = 2; 
        s.description = "Collision, so idx++ (Linear Probing)!";
        steps.push_back(s);
    }

    s.codeLine = 4;
    s.description = "Check the last condition!";
    steps.push_back(s);
    
    // Bước cuối: kiểm tra table[idx]
    if (table->isOccupied(idx) && table->getKeyAt(idx) == key) {
        s.highlightedIndex = idx;
        s.codeLine = 5;
        s.description = "Set the occurence of key is false!";
        steps.push_back(s);

        s.highlightedIndex = idx;
        s.codeLine = 6;
        s.description = "The key was deleted from the table!";
        steps.push_back(s);
    } else {
        s.highlightedIndex = -1;
        s.codeLine = 8;
        s.description = "Key was not founded!";
        steps.push_back(s);
    }
}

void HashTablePage::buildSearchSteps(int key) {
    steps.clear();
    currentStep = 0;

    int idx = key % table->getTableSize();
    Step s;
    
    int cnt = 0;
    // Bước 1: cnt = 0
    s.highlightedIndex = idx;
    s.codeLine = 0; 
    s.description = "Initialize cnt = 0;";
    steps.push_back(s);

    // Bước 2: Tính idx
    s.highlightedIndex = idx;
    s.codeLine = 1; 
    s.description = "Compute idx = key % TABLE_SIZE = " + std::to_string(key) + " % " + std::to_string(table->getTableSize());
    steps.push_back(s);

    // Bước 3+: Vòng while
    while (table->isOccupied(idx) && table->getKeyAt(idx) != key) {
        // Mỗi vòng lặp => 1 bước
        s.highlightedIndex = idx;
        s.codeLine = 2;
        s.description = "Check condition occ[idx] && table[idx] != key";
        steps.push_back(s);

        // Bước 3b: idx = (idx + 1) % TABLE_SIZE;
        idx = (idx + 1) % table->getTableSize();
        cnt++;
        s.highlightedIndex = idx;
        s.codeLine = 3; 
        s.description = "Collision, so idx++ (Linear Probing)!";
        steps.push_back(s);

        // Bước 3c: cnt++
        s.highlightedIndex = idx;
        s.codeLine = 4;
        s.description = "Increment cnt!";
        steps.push_back(s);

        // Bước 3d: if (cnt == TABLE_SIZE) return false;
        if (cnt == table->getTableSize()) {
            s.highlightedIndex = -1;
            s.codeLine = 5;
            s.description = "The table is full, key was not founded!";
            steps.push_back(s);
            return; // Kết thúc buildSteps
        }
    }

    s.codeLine = 7;
    s.description = "Check the last condition!";
    steps.push_back(s);
    
    // Bước cuối: kiểm tra table[idx]
    if (table->isOccupied(idx) && table->getKeyAt(idx) == key) {
        s.highlightedIndex = idx;
        s.codeLine = 8;
        s.description = "Key was founded!";
        steps.push_back(s);
    } else {
        s.highlightedIndex = -1;
        s.codeLine = 10;
        s.description = "Key was not founded!";
        steps.push_back(s);
    }
}