#include "../header/HashTableVisual.h"


void DisplayHash() {
    DrawRectangle(0, 0, 400, 100, Color{248, 186, 200, 255});
    DrawText("BACK", 200 - MeasureText("BACK", 10), 40, 20, Color{255, 254, 206, 255});
}

HashTablePage::HashTablePage() : 
    inputField((screenWidth - 400)/2, 100, 400, 50),
    filePathInput((screenWidth - 400)/2, (screenHeight - 50)/2, 400, 50),
    table(nullptr),
    startX(450), startY(170), offsetX(30),
    cellSize(80),
    highlightedIdx(-1),

    createButton(10, 170, 250, 40, "Create Table", 30), 
    insertButton(10, 220, 250, 40, "Insert", 30),
    updateButton(270, 220, 100, 40, "Update", 30),
    deleteButton(10, 270, 250, 40, "Delete", 30),
    searchButton(10, 320, 250, 40, "Search", 30),
    clearButton(10, 370, 250, 40, "Clear", 30),
    newButton(10, 420, 250, 40, "New", 30),
    loadFileButton(10, 470, 250, 40, "Load File", 30),
    randomButton(10, 520, 250, 40, "Random", 30),
    closeLoadFileButton((screenWidth - 400)/2.0 + 400, (screenHeight - 50)/2.0, 100, 50, "Close", 30),
    speedSlider(1200 + 30/2 + (300 - 2*15)/4, 100 + 30/2, 30/2),

    stepModeButton(10, 570, 250, 40, "Run Step by Step", 30),
    OnOffButton(270, 570, 100, 40, "Off", 30),
    browseButton(270, 470, 100, 40, "Browse", 30),
    playButton(380, 570, 100, 40, "Pause", 30),

    tableCreated(false),
    stepModeOn(false),
    insertStepModeOn(false),
    deleteStepModeOn(false),
    searchStepModeOn(false),
    currentStep(0)
{ 
    filePathInput.setActive(false);

    codeLinesForInsert = {
        "int cnt = 0;",
        "if (search(key, idx))", 
        "   return;",
        "idx = key % TABLE_SIZE;",
        "while (occ[idx] = 1) {",
        "   idx = (idx + 1) % TABLE_SIZE;",
        "   cnt++;",
        "   if (cnt == TABLE_SIZE) return;",
        "}",
        "table[idx] = key;",
        "occ[idx] = 1;"
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
        "if (occ[idx] = 1 && table[idx] == key) {",
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
            int key = std::rand() % 100000, idx_ =-1 ;
            table->insert(key, idx_);
        }
        highlightedIdx = -1;
        tableCreated = true;
        filePathInput.setActive(false);
        inputField.setActive(true);
    } 

    browseButton.handleInput();
    if (browseButton.isClicked()) {
        const char* filterPatterns[1] = { "*.txt" };

        // Hiển thị hộp thoại mở file
        const char* filePath = tinyfd_openFileDialog(
            "Chọn tệp cần mở",    // Tiêu đề của hộp thoại
            "",                   // Đường dẫn mặc định (để trống nếu không có)
            1,                    // Số lượng filter
            filterPatterns,       // Mảng các filter
            "Text files (*.txt)", // Mô tả filter
            0                     // Kiểu hộp thoại (0: mở file, 1: mở nhiều file)
        );
        
        // Kiểm tra nếu người dùng đã chọn file
        if (filePath) {
            if (table == nullptr) {           // Tránh gây crash khi gọi loadHashTableFromFile()
                table = new HashTable(1);     // Size tạm, sẽ được cập nhật khi load file
            }
            table->loadHashTableFromFile(std::string(filePath));
            tableCreated = true;
            highlightedIdx = -1;
            filePathInput.setActive(false);
        } else {
            TraceLog(LOG_WARNING, "Cannot open the file!");
        }
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
    updateButton.handleInput();
    deleteButton.handleInput();
    searchButton.handleInput();
    clearButton.handleInput();
    newButton.handleInput();
    OnOffButton.handleInput();
    playButton.handleInput();

    if (OnOffButton.isClicked()) {
        stepModeOn = !stepModeOn;
        if (stepModeOn) {
            OnOffButton.setText("On");
        } else {
            OnOffButton.setText("Off");
        }
        highlightedIdx = -1;
    }

    if (stepModeOn) {
        // Nhấn Next => currentStep++
        if (IsKeyPressed(KEY_RIGHT)) {
            if (insertStepModeOn) {
                if (currentStep < (int)steps.size() - 1) {
                    currentStep++;
                    if (currentStep == (int)steps.size() - 2) {
                        int idx_ = -1;
                        table->insert(pendingInsertKey, idx_);
                    }
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
                    int idx_ = -1;
                    table->insert(pendingInsertKey, idx_);
                }
            } 
            if (searchStepModeOn) {
                // Don't need to do anything
            }

            if (currentStep > 0) {
                currentStep--;
            }
        }

        // Xử lý thanh speedSlider
        speedSlider.handleInput();
        if (speedSlider.isHold()) {
            speedSlider.setX(Clamp(GetMousePosition().x, 1200 + 30/2, 1500 - 30/2)); 
            playSpeed = ((speedSlider.getX() - (1200 + 30/2)) / (300 - 2*15)) * playSpeedMax;
        }
    }

    if (insertButton.isClicked() || IsKeyPressed(KEY_ENTER))
    {
        std::string str = inputField.getText();
        if (!str.empty())
        {
            try
            {
                int key = std::stoi(str);
                if (!stepModeOn) {
                    int idx_ = -1;
                    if (table->insert(key, idx_))
                        highlightedIdx = idx_;
                } else {
                    buildInsertSteps(key);
                    insertStepModeOn = true;
                    deleteStepModeOn = searchStepModeOn = false;
                }
            }
            catch (...)
            {

            }

            inputField.clearText();
        }
    }

    if (updateButton.isClicked()) {
        std::string str = inputField.getText();
        if (!str.empty()) {
            inputField.clearText();
            try {
                int newKey = std::stoi(str);
                table->update(newKey);
                highlightedIdx = newKey % table->getTableSize();
            } catch(...) {

            }
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

    if (playButton.isClicked()) {
        isPlaying = !isPlaying;
        if (isPlaying)
            playButton.setText("Play");
        else
            playButton.setText("Pause");
    }
}

void HashTablePage::update(float deltaTime) 
{
    inputField.update(deltaTime);
    filePathInput.update(deltaTime);

    if (stepModeOn && isPlaying && steps.size() > 0) 
    {
        animationProgress += playSpeed * deltaTime;
        if (animationProgress >= 1.0f) {
            animationProgress = 0.0f;

            if (currentStep < (int)steps.size() - 1) {
                currentStep++;
                if (insertStepModeOn && currentStep == (int)steps.size() - 2) {
                    int idx_ = -1;
                    table->insert(pendingInsertKey, idx_);
                }
                if (deleteStepModeOn && currentStep == (int)steps.size() - 2) {
                    int temp = -1;
                    table->remove(pendingInsertKey, temp);
                }
                // Nothing to do with searchStepModeOn
            } 
        }
        progressValue = (float)currentStep / (steps.size() - 1) + (animationProgress / (steps.size() - 1));
    } 
    else if (stepModeOn && !isPlaying && steps.size() > 0) 
    {
        progressValue = (float)currentStep / (steps.size() - 1);
    }
}

void HashTablePage::draw() 
{
    // BeginDrawing();
    ClearBackground((Color){241, 231, 231, 255});
    createButton.draw();
    insertButton.draw();
    updateButton.draw();
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
    playButton.draw();

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

    // Vẽ trước các ô vuông (Layer 1)
    if (tableCreated && table != nullptr)
    {
        int tableSize = table->getTableSize();
        for (int i = 0; i < tableSize; i++) // Traverse the whole table
        {
            float cellX = startX + (i % 10 + 0.5)*(cellSize + offsetX);
            float cellY = startY + (i / 10)*(cellSize + 40);

            Color cellColor = (i == highlightedIdx) ? Color {255, 254, 206, 255} : RAYWHITE;
            DrawRectangle(cellX, cellY, cellSize, cellSize, cellColor);
            Color lineColor = (i == highlightedIdx) ? RED : BLACK;
            DrawRectangleLines(cellX, cellY, cellSize, cellSize, lineColor);
        }
    }

    // Vẽ ô highlighted với animation chuyển động (nếu đang ở chế độ stepModeOn) (Layer 2)
    if (tableCreated && table != nullptr && stepModeOn && steps.size() > 0) {
        int currentIndex = steps[currentStep].highlightedIndex;
        Vector2 currentPos = {startX + (currentIndex%10 + 0.5f)*(cellSize + offsetX), startY + (currentIndex/10)*(cellSize + 40)};
        // Tính vị trí của ô tiếp theo nếu có bước kế tiếp
        Vector2 targetPos = currentPos;
        if (currentStep < (int)steps.size() - 1) {
            int nextIndex = steps[currentStep + 1].highlightedIndex;
            targetPos = {startX + (nextIndex%10 + 0.5f)*(cellSize + offsetX), startY + (nextIndex/10)*(cellSize + 40)};
        }
        // Vị trí theo animationProgress 
        Vector2 animPos = {
            currentPos.x + (targetPos.x - currentPos.x) * animationProgress,
            currentPos.y + (targetPos.y - currentPos.y) * animationProgress
        };
        // Vẽ ô highlight tại vị trí animPos (cùng kích thước ô vuông)
        DrawRectangle(animPos.x, animPos.y, 80, 80, Color{255, 254, 206, 255});
        DrawRectangleLines(animPos.x, animPos.y, 80, 80, RED);
    }

    // Nếu bảng đã được tạo, vẽ key và index (Layer 3)
    if (tableCreated && table != nullptr)
    {
        int tableSize = table->getTableSize();
        for (int i = 0; i < tableSize; i++) // Traverse the whole table
        {
            float cellX = startX + (i % 10 + 0.5)*(cellSize + offsetX);
            float cellY = startY + (i / 10)*(cellSize + 40);

                if (table->isOccupied(i) == 1)
                {
                std::string keyStr = std::to_string(table->getKeyAt(i));
                int fontSize = 25;
                Vector2 textSizeKey = MeasureTextEx(FONT, keyStr.c_str(), fontSize, 1);
                DrawTextEx(FONT, keyStr.c_str(), 
                            Vector2{ cellX + (cellSize - textSizeKey.x) / 2, cellY + (cellSize - textSizeKey.y) / 2 },
                            fontSize, 1, BLACK);
                }
                if (table->isOccupied(i) == 2)
                {
                    DrawLine(cellX, cellY, cellX + 80, cellY + 80, BLACK);   
                    DrawLine(cellX + 80, cellY, cellX, cellY + 80, BLACK); 
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
        DrawRectangle(0, 620, 450,screenHeight - 620, Color {248, 186, 200, 255});
        DrawRectangleLines(0, 620, 450, screenHeight - 620, Color {194, 24, 91, 255});

        // Nếu stepModeOn == true, highlight dòng code = steps[currentStep].codeLine
        int highlightLine = -1;
        if (stepModeOn && currentStep >= 0 && currentStep < (int)steps.size()) {
            highlightLine = steps[currentStep].codeLine;
        }

        // Vẽ từng dòng code
        float lineHeight = 20.0f;
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
            DrawTextEx(FONT, curCodeLines[i].c_str(), {5, lineY}, 20, 1, BLACK);
        }

        // Nếu muốn vẽ mô tả step hiện tại
        if (stepModeOn && currentStep < (int)steps.size()) {
            DrawTextEx(FONT, steps[currentStep].description.c_str(), {5, float(screenHeight) - 25}, 20, 1, RED);
        }

        // Vẽ thanh progressBar dọc 
        int barWidth = 30;
        int barHeight = screenHeight - 620; 
        int barX = 450;
        int barY = 620;

        DrawRectangleLines(450, 620, 30, screenHeight - 620, Color {194, 24, 91, 255});

        // Vẽ phần đã hoàn thành (filled) từ dưới lên
        barHeight--;
        int filledHeight = ((int)(barHeight * progressValue) > barHeight) ? barHeight : (int)(barHeight * progressValue);
        // Vì progress bar chạy từ dưới lên nên vị trí y của phần filled sẽ là barY + (barHeight - filledHeight)
        DrawRectangle(barX + 1, barY + 1 + (barHeight - filledHeight), barWidth - 2, filledHeight, Color{245, 162, 178, 255});

        // Vẽ thanh playSpeed slider nằm ngang
        int playSpeedWidth = MeasureTextEx(FONT, "Playing Speed", 30, 1).x;
        DrawTextEx(FONT, "Playing Speed", {1200 + 300/2 - playSpeedWidth/2.0f, 50}, 30, 1, Color{87, 143, 202, 255});
        DrawRectangleRounded({1200, 100, 300, 30}, 1.0f, 10, Color {248, 186, 200, 255});
        DrawRectangleRoundedLines({1200, 100, 300, 30}, 1.0f, 10, Color {194, 24, 91, 255});

        // Vẽ nút slider
        speedSlider.draw();
    }

    // EndDrawing();
}

void HashTablePage::buildInsertSteps(int key) {
    pendingInsertKey = key;
    steps.clear();
    currentStep = 0;

    int idx = key % table->getTableSize();
    Step s;

    s.highlightedIndex = idx;
    s.codeLine = 0; 
    s.description = "Initialize cnt = 0!";
    steps.push_back(s);

    // Bước 1: Checking the existence
    s.highlightedIndex = idx;
    s.codeLine = 1; 
    s.description = "Checking the existence...";
    steps.push_back(s);
    if (table->search(key, idx)) {
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
    int cnt = 0;
    while (table->isOccupied(idx) == 1) {
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

        // Bước 3c: cnt++
        cnt++;
        s.highlightedIndex = idx;
        s.codeLine = 6;
        s.description = "Increment cnt!";
        steps.push_back(s);

        // Bước 3d: if (cnt == TABLE_SIZE) return false;
        if (cnt == table->getTableSize()) {
            s.highlightedIndex = -1;
            s.codeLine = 7;
            s.description = "The table is full, key is not inserted!";
            steps.push_back(s);
            return; // Kết thúc buildSteps
        }
    }

    s.highlightedIndex = idx;
    s.codeLine = 9;
    s.description = "Add key into the table!";
    steps.push_back(s);
    
    s.highlightedIndex = idx;
    s.codeLine = 10;
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

    s.highlightedIndex = idx;
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
    s.highlightedIndex = -1;
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
        s.highlightedIndex = idx;
        s.codeLine = 3; 
        s.description = "Collision, so idx++ (Linear Probing)!";
        steps.push_back(s);

        // Bước 3c: cnt++
        cnt++;
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
    if (table->isOccupied(idx) == 1 && table->getKeyAt(idx) == key) {
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