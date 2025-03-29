#include "../header/HashTableVisual.h"

HashTablePage::HashTablePage(Program* prog) : 
    program(prog),
    inputField((Global::WINDOW_WIDTH - 400)/2, 100, 400, 50),
    table(nullptr),
    startX(10 + 150), startY(170), offsetX((800 - startX - 6*50)/7),
    highlightedIdx(-1),
    backButton(0, 0, 100, 40, "Back", 25),
    createButton(10, 200, 180, 40, "Create Table", 30), // Modidy the coordinate after
    insertButton(10, 250, 180, 40, "Insert", 30),
    deleteButton(10, 300, 180, 40, "Delete", 30),
    searchButton(10, 350, 180, 40, "Search", 30),
    clearButton(10, 400, 180, 40, "Clear", 30),
    newButton(10, 450, 180, 40, "New", 30),
    tableCreated(false)
{ 
    backButton.setIdleColor(LIGHTGRAY);
    createButton.setIdleColor(LIGHTGRAY);
    insertButton.setIdleColor(LIGHTGRAY);
    deleteButton.setIdleColor(LIGHTGRAY);
    searchButton.setIdleColor(LIGHTGRAY);
    clearButton.setIdleColor(LIGHTGRAY);
    newButton.setIdleColor(LIGHTGRAY);
}

void HashTablePage::handleInput() 
{
    // Xử lý nhập
    inputField.handleInput();
    backButton.handleInput();

    // Nếu nhấn Back -> quay về MainPage
    if (backButton.isClicked())
    {
        program->popPage();
        return;
    }

    if (!tableCreated) {
        // Chưa tạo bảng, chỉ xử lý nút Create Table
        createButton.handleInput();
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

        return;
    }

    insertButton.handleInput(); // Should be handled after creation
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
    BeginDrawing();
    ClearBackground(Color {135, 206, 250});
    backButton.draw();
    inputField.draw();
    createButton.draw();
    insertButton.draw();
    deleteButton.draw();
    searchButton.draw();
    clearButton.draw();
    newButton.draw();

    // Vẽ tiêu đề và hướng dẫn
    if (!tableCreated)
    {
        // Khi chưa tạo bảng, yêu cầu người dùng nhập kích thước của bảng
        const char *text = "Enter table size:";
        Vector2 textSize = MeasureTextEx(Global::mainFont, text, 35, 1);
        DrawTextEx(Global::mainFont, text, Vector2{(Global::WINDOW_WIDTH - textSize.x)/2, 50}, 35, 1, WHITE);
    }
    else
    {
        const char *text = "Insert values into the below box!";
        Vector2 textSize = MeasureTextEx(Global::mainFont, text, 35, 1);
        DrawTextEx(Global::mainFont, text, Vector2{(Global::WINDOW_WIDTH - textSize.x)/2, 50}, 35, 1, WHITE);
    }

    // Nếu bảng đã được tạo, biểu diễn HashTable dưới dạng các ô vuông
    if (tableCreated && table != nullptr)
    {
        int tableSize = table->getTableSize();
        const int cellSize = 50;  
        for (int i = 0; i < tableSize; i++) // Traverse the whole table
        {
            float cellX = startX + (i % 6 + 0.5)*(cellSize + offsetX);
            float cellY = startY + (i / 6)*(cellSize + 30);

            Color cellColor = (i == highlightedIdx) ? YELLOW : RAYWHITE;
            DrawRectangle(cellX, cellY, cellSize, cellSize, cellColor);
            DrawRectangleLines(cellX, cellY, cellSize, cellSize, BLACK);

            if (table->isOccupied(i))
            {
                std::string keyStr = std::to_string(table->getKeyAt(i));
                int fontSize = 25;
                Vector2 textSizeKey = MeasureTextEx(Global::mainFont, keyStr.c_str(), fontSize, 1);
                DrawTextEx(Global::mainFont, keyStr.c_str(), 
                           Vector2{ cellX + (cellSize - textSizeKey.x) / 2, cellY + (cellSize - textSizeKey.y) / 2 },
                           fontSize, 1, BLACK);
            }

            std::string idxStr = std::to_string(i);
            int idxFontSize = 20;
            Vector2 idxTextSize = MeasureTextEx(Global::mainFont, idxStr.c_str(), idxFontSize, 1);
            DrawTextEx(Global::mainFont, idxStr.c_str(), 
                       Vector2{ cellX + (cellSize - idxTextSize.x) / 2, cellY + cellSize + 5 },
                       idxFontSize, 1, BLACK);
        }
    }

    EndDrawing();
}