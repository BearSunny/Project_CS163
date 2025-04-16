#ifndef MAINMENU_H
#define MAINMENU_H

#include "declare.h"
#include "LinkedListVisualizer.h"
#include "hashvisual.h"
#include "HashTableVisual.h"
#include "graphvisual.h"
#include "treevisual.h"
/* #include "../source/LinkedListVisualizer0.cpp"
#include "../source/LinkedList0.cpp"
#include "../source/treevisual.cpp" */
/* #include "../source/Graph.cpp"
#include "../source/GraphButton.cpp" */
/* #include "../source/HashTableVisual.cpp"
 */
// void DisplayMainMenu();
// void MAINMENU_INTERACT();

const int NUM_MODES = 4;

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
    bool isTransitioning;   
    float transitionTime; 
    float elapsedTime;      

    Texture2D logoTexture, logoTextureNight;

public:
    MainPage();
    ~MainPage();

    void handleInput();
    
    void update(float deltaTime);

    void draw();
};

#endif