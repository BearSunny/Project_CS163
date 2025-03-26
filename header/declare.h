#ifndef DECLARE_H
#define DECLARE_H

#include <raylib.h>
#include <bits/stdc++.h>

using namespace std;

// Enums and structs
typedef enum GameScreen { MAINMENU, LINKEDLIST, HASH, TREE, GRAPH } GameScreen;

struct theme {
    Color BACKGROUND;
    Color MENU_BUTTON;
    Color HOVER_MENU;
    Color BUTTON;
    Color HOVER_BUTTON;
    Color SEPERATOR;
    Color INFOR;
    Color HIGHLIGHT_TEXT;
    Color HIGHLIGHT_NODE_1;
    Color HIGHLIGHT_NODE_2;
    Color NODE;
    Color LINE;
    Color SIDEBAR;
    Color WEIGHT;
    bool isDarkMode = true;
};

// Global variable declarations
extern theme THEME;
extern Font FONT;
extern Texture2D PLAY, PAUSE, REPLAY, BACK, DOUBLE_BACK, NEXT, DOUBLE_NEXT, SPEED_UP, SPEED_DOWN, BACK_PAGE;

extern const int screenWidth;
extern const int screenHeight;
extern const Vector2 ORIGIN;
extern const float CODE_SIZE;

extern GameScreen currentScreen;
extern bool InsertButton, DeleteButton, FindButton;
extern int FindQuerry, FindNumber, framecount, length;
extern char number[10];

// Function declarations
void initResource();
void Insertnumber(int x);
void Deletenumber();
void DrawNumber(int data, int x, int y, int fs);
bool MouseButtonPressed(float x, float y, float u, float v);
void Updatenumber();
void drawPicture(const char* path, Rectangle desRec, float rotation, Vector2 origin, Color color);
void drawPicture(Texture2D texture, Rectangle desRec, float rotation, Vector2 origin, Color color);

#endif