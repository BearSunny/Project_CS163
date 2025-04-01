#include "../header/HashTableVisual.h"

HashTablePage::HashTablePage() : 
    inputField((screenWidth - 400)/2, 100, 400, 50),
    filePathInput((screenWidth - 400)/2, (screenHeight - 50)/2, 400, 50),
    table(nullptr),
    startX(150 + 150), startY(170), offsetX((800 - startX - 6*50)/7),
    highlightedIdx(-1),

    createButton(10, 200, 180, 40, "Create Table", 30), // Modidy the coordinate after
    insertButton(10, 270, 180, 40, "Insert", 30),
    deleteButton(10, 340, 180, 40, "Delete", 30),
    searchButton(10, 410, 180, 40, "Search", 30),
    clearButton(10, 480, 180, 40, "Clear", 30),
    newButton(10, 550, 180, 40, "New", 30),
    loadFileButton(10, 620, 180, 40, "Load File", 30),
    randomButton(10, 690, 180, 40, "Random", 30),
    tableCreated(false)
{ 
    filePathInput.setActive(false);
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
        inputField.handleInput();
    }

    if (!tableCreated) {
        // Chưa tạo bảng, chỉ xử lý nút Create Table và Load File
        createButton.handleInput();
        loadFileButton.handleInput();

        if (createButton.isClicked())
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

        return;
    }

    // Should be handled after creation
    insertButton.handleInput(); 
    deleteButton.handleInput();
    searchButton.handleInput();
    clearButton.handleInput();
    newButton.handleInput();

    if (insertButton.isClicked())
    {
        std::string str = inputField.getText();
        if (!str.empty())
        {
            try
            {
                int key = std::stoi(str);
                table->insert(key);
                highlightedIdx = -1; // reset highlight
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
                int idx;
                if (table->remove(key, idx))
                {
                    // Nếu xóa thành công, có thể highlight ô vừa xóa (màu khác) trong một khoảng thời gian ngắn
                    highlightedIdx = idx;
                }
                else
                {
                    highlightedIdx = -1;
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
                int idx;
                if (table->search(key, idx))
                {
                    highlightedIdx = idx;
                }
                else
                {
                    highlightedIdx = -1;
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
}

void HashTablePage::update(float deltaTime) 
{
    // Không cần update gì nhiều, 
    // có thể thêm animation cho square nếu muốn
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
    if (filePathInput.IsActive()) {
        const char *text = "Drag and drop input file or type the file path into the below box";
        Vector2 textSize = MeasureTextEx(FONT, text, 35, 1);
        DrawTextEx(FONT, text, Vector2{(screenWidth - textSize.x)/2, 375}, 35, 1, Color{87, 143, 202, 255});
        filePathInput.draw();
    }

    // Vẽ tiêu đề và hướng dẫn
    if (!tableCreated)
    {
        // Khi chưa tạo bảng, yêu cầu người dùng nhập kích thước của bảng
        const char *text = "Please enter table size first";
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
        int tableSize = table->getTableSize();
        const int cellSize = 80;  
        for (int i = 0; i < tableSize; i++) // Traverse the whole table
        {
            float cellX = startX + (i % 10 + 0.5)*(cellSize + offsetX);
            float cellY = startY + (i / 10)*(cellSize + 40);

            Color cellColor = (i == highlightedIdx) ? YELLOW : RAYWHITE;
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

    // EndDrawing();
}