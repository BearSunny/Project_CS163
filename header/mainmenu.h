#ifndef MAINMENU_H
#define MAINMENU_H

#include "declare.h"
#include "LinkedListVisualizer.h"
#include "hashvisual.h"
#include "header/HashTableVisual.h"
#include "../source/LinkedListVisualizer0.cpp"
#include "../source/LinkedList0.cpp"
#include "../source/treevisual.cpp"
#include "../source/Graph.cpp"
#include "../source/GraphButton.cpp"
#include "../source/HashTableVisual.cpp"

// void DisplayMainMenu();
// void MAINMENU_INTERACT();

const int NUM_MODES = 4;

float deltaTime;

enum class DSMode {
    LINKEDLIST,
    HASH,
    TREE,
    GRAPH
};

struct ModeTexture {
    Texture2D texture;      // Hình ảnh mode
    Vector2 position;       // Vị trí hiện tại
    float scale;            // Tỉ lệ hiện tại
    unsigned char alpha;    // Transparency

    Vector2 startPosition;
    float startScale;
    Vector2 targetPosition;
    float targetScale;
};

// Lớp MainPage
class MainPage {
private:
    // Tab Data Structure
    HashButton dsModeButton;   
    DSMode dsMode; 
    std::vector<ModeTexture> modes;
    bool isTransitioning;   // Flag đánh dấu đang chuyển động hay không
    float transitionTime;   // Tổng thời gian chuyển động (giây)
    float elapsedTime;      // Thời gian đã trôi qua kể từ khi bắt đầu chuyển động

public:
    MainPage();
    ~MainPage();

    void handleInput();
    
    void update(float deltaTime);

    void draw();
};

#endif