#include "../header/mainmenu.h"
#include "header/LinkedListVisualizer.h"

// void DisplayMainMenu() {
//     DrawRectangle(0, 0, 500, 400, LIGHTGRAY);
//     DrawText("LINKED LIST", 250 - MeasureText("LINKED LIST", 40) / 2, 180, 40, DARKBROWN);

//     DrawRectangle(500, 0, 500, 400, GRAY);
//     DrawText("HASH", 750 - MeasureText("HASH", 40) / 2, 180, 40, BROWN);

//     DrawRectangle(0, 400, 500, 400, DARKGRAY);
//     DrawText("TREE", 250 - MeasureText("TREE", 40) / 2, 580, 40, BEIGE);

//     DrawRectangle(500, 400, 500, 400, YELLOW);
//     DrawText("GRAPH", 750 - MeasureText("GRAPH", 40) / 2, 580, 40, DARKPURPLE);
// }

// void MAINMENU_INTERACT() {
//     if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
//         if(MouseButtonPressed(0, 0, 500, 400)) {
//             currentScreen = LINKEDLIST;
//             InitLinkedList();
//         }
//         else if(MouseButtonPressed(500, 0, 1000, 400))
//             currentScreen = HASH;
//         else if(MouseButtonPressed(0, 400, 500, 800))
//             currentScreen = TREE;
//         else if(MouseButtonPressed(500, 400, 1000, 800))
//             currentScreen = GRAPH;
//     }
// }

MainPage::MainPage() : 
    dsModeButton(650, 386, 299, 375, "", 1),
    dsMode(DSMode::LINKEDLIST),
    isTransitioning(false),
    transitionTime(0.5f), 
    elapsedTime(0.0f)
{
    logoTexture = LoadTexture("asset/DS Visualizer Logo.png");

    modes.resize(NUM_MODES);
    modes[0].texture = LoadTexture("asset/LinkedListMenu.png");
    modes[1].texture = LoadTexture("asset/HashTableMenu.png");
    modes[2].texture = LoadTexture("asset/AVLTreeMenu.png");
    modes[3].texture = LoadTexture("asset/GraphMenu.png");
    
    // Initialize position, scale, alpha for each mode texture
    modes[0].targetPosition = {dsModeButton.getX(), dsModeButton.getY()};
    modes[0].targetScale = 1;
    modes[0].alpha = 255;
    modes[1].targetPosition = {972, 419};
    modes[1].targetScale = 0.83f;
    modes[1].alpha = 205;
    modes[2].targetPosition = {1243, 465};
    modes[2].targetScale = 0.58f;
    modes[2].alpha = 155;
    modes[3].targetPosition = {1439, 487};
    modes[3].targetScale = 0.47f;
    modes[3].alpha = 105;
}

MainPage::~MainPage()
{
    for (auto &mode : modes) {
        UnloadTexture(mode.texture);
    }
}

void MainPage::handleInput() 
{
    dsModeButton.handleInput();

    if (IsKeyPressed(KEY_RIGHT)) {
        int next = static_cast<int>(dsMode) + 1;
        next %= static_cast<int>(NUM_MODES);
        dsMode = static_cast<DSMode>(next);
    }
    if (IsKeyPressed(KEY_LEFT)) {
        int next = static_cast<int>(dsMode) + 3;
        next %= static_cast<int>(NUM_MODES);
        dsMode = static_cast<DSMode>(next);
    }

    for (int i = 0; i < NUM_MODES; i++) {
        modes[i].startPosition = modes[i].position;
        modes[i].startScale = modes[i].scale;
    }
    switch (dsMode) {
        case DSMode::LINKEDLIST:
            modes[0].targetPosition = {dsModeButton.getX(), dsModeButton.getY()};
            modes[0].targetScale = 1;
            modes[0].alpha = 255;
            modes[1].targetPosition = {972, 419};
            modes[1].targetScale = 0.83f;
            modes[1].alpha = 205;
            modes[2].targetPosition = {1243, 465};
            modes[2].targetScale = 0.58f;
            modes[2].alpha = 155;
            modes[3].targetPosition = {1439, 487};
            modes[3].targetScale = 0.47f;
            modes[3].alpha = 105;
            break;
        case DSMode::HASH:
            modes[1].targetPosition = {dsModeButton.getX(), dsModeButton.getY()};
            modes[1].targetScale = 1;
            modes[1].alpha = 255;
            modes[2].targetPosition = {972, 419};
            modes[2].targetScale = 0.83f;
            modes[2].alpha = 205;
            modes[3].targetPosition = {1243, 465};
            modes[3].targetScale = 0.58f;
            modes[3].alpha = 155;
            modes[0].targetPosition = {380, 419};
            modes[0].targetScale = 0.83f;
            modes[0].alpha = 205;
            break;
        case DSMode::TREE:
            modes[2].targetPosition = {dsModeButton.getX(), dsModeButton.getY()};
            modes[2].targetScale = 1;
            modes[2].alpha = 255;
            modes[3].targetPosition = {972, 419};
            modes[3].targetScale = 0.83f;
            modes[3].alpha = 205;
            modes[0].targetPosition = {184, 465};
            modes[0].targetScale = 0.58f;
            modes[0].alpha = 155;
            modes[1].targetPosition = {380, 419};
            modes[1].targetScale = 0.83f;
            modes[1].alpha = 205;
            break;
        case DSMode::GRAPH:
            modes[3].targetPosition = {dsModeButton.getX(), dsModeButton.getY()};
            modes[3].targetScale = 1;
            modes[3].alpha = 255;
            modes[0].targetPosition = {21, 487};
            modes[0].targetScale = 0.47f;
            modes[0].alpha = 105;
            modes[1].targetPosition = {184, 465};
            modes[1].targetScale = 0.58f;
            modes[1].alpha = 155;
            modes[2].targetPosition = {380, 419};
            modes[2].targetScale = 0.83f;
            modes[2].alpha = 205;
            break;
        default:
            break;
    }
    elapsedTime = 0.0f;
    isTransitioning = true;

    // Kiểm tra click
    if (dsModeButton.isClicked() || IsKeyPressed(KEY_ENTER)) {
        switch (dsMode) {
            case DSMode::LINKEDLIST:    
                currentScreen = LINKEDLIST;
                InitLinkedList();
                break;
            case DSMode::HASH:
                currentScreen = HASH;
                break;
            case DSMode::TREE:
                currentScreen = TREE;
                break;
            case DSMode::GRAPH:
                currentScreen = GRAPH;
                break;
            default:
                break;
        }
    }
}

void MainPage::update(float deltaTime) 
{
    if (isTransitioning) {
        elapsedTime += deltaTime;
        // Tính t với giới hạn max = 1.0 
        float t = elapsedTime / transitionTime;
        if (t >= 1.0f) {
            t = 1.0f;
            isTransitioning = false;
        }

        // Cập nhật vị trí và scale cho từng mode 
        for (auto &mode : modes) {
            mode.position.x = mode.startPosition.x + (mode.targetPosition.x - mode.startPosition.x) * t;
            mode.position.y = mode.startPosition.y + (mode.targetPosition.y - mode.startPosition.y) * t;
            mode.scale = mode.startScale + (mode.targetScale - mode.startScale) * t;
        }
    }
}

void MainPage::draw() 
{
    DrawTexture(logoTexture, (screenWidth - (float)logoTexture.width)/2.0f, 30, WHITE);
    Vector2 text = MeasureTextEx(MONT, "GROUP 5 - PROJECT CS163", 40, 2);
    DrawTextEx(MONT, "GROUP 5 - PROJECT CS163", {(screenWidth - (float)text.x)/2.0f, 300}, 40, 2, Color{16, 134, 244, 255});

    for (const auto mode : modes) {
        DrawTextureEx(mode.texture, mode.position, 0, mode.scale, (Color){255, 255, 255, mode.alpha});
    }

    for (int i = 0; i < 4; i++) {
        DrawCircleLines(681 + i*80, 828, 20, BLACK);
        DrawCircleLines(681 + i*80, 828, 19, BLACK);
    }
    DrawCircle(681 + static_cast<int>(dsMode) * 80, 828, 19, Color {245, 162, 178, 255});
}
