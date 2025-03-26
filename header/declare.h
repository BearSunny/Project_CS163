#ifndef DECLARE_H
#define DECLARE_H

#include <raylib.h>
#include <bits/stdc++.h>

using namespace std;

typedef enum GameScreen {MAINMENU, LINKEDLIST, HASH, TREE, GRAPH} GameScreen;

inline Font FONT = LoadFontEx("./Font/SF-Pro-Display-Regular.otf", 40, 0, 255);

const int screenWidth = 1000;
const int screenHeight = 800;

GameScreen currentScreen = MAINMENU;

bool InsertButton = false;
bool DeleteButton = false;
bool FindButton = false;
int FindQuerry = -1;
int FindNumber = 0;
int framecount = 0;
char number[10] = {};
int length = 0;

void Insertnumber(int x) {
    if(length == 4)
        return;
    number[length] = x + '0';
    ++length;
    number[length] = 0;
}

void Deletenumber() {
    if(length == 0)
        return;
    --length;
    number[length] = 0;
}

void DrawNumber(int data, int x, int y, int fs) {
    int len = 0;
    int tmp = data;
    while(data) {
        ++len;
        data /= 10;
    }
    if(len == 0) {
        DrawText("0", x - MeasureText("0", fs) / 2, y - fs / 2, fs, BLACK);
        return;
    }
    char *c = new char[len + 1];
    c[len] = 0;
    while(tmp) {
        --len;
        c[len] = tmp % 10 + '0';
        tmp /= 10;
    }
    DrawText(c, x - MeasureText(c, fs) / 2, y - fs / 2, fs, BLACK);
    delete[] c;
}

bool MouseButtonPressed(float x, float y, float u, float v) {
    Vector2 p = GetMousePosition();
    if(p.x >= x && p.x < u && p.y >= y && p.y < v)
        return true;
    return false;
}

void Updatenumber() {
    if(IsKeyPressed(KEY_ZERO))
        Insertnumber(0);
    else if(IsKeyPressed(KEY_ONE))
        Insertnumber(1);
    else if(IsKeyPressed(KEY_TWO))
        Insertnumber(2);
    else if(IsKeyPressed(KEY_THREE))
        Insertnumber(3);
    else if(IsKeyPressed(KEY_FOUR))
        Insertnumber(4);
    else if(IsKeyPressed(KEY_FIVE))
        Insertnumber(5);
    else if(IsKeyPressed(KEY_SIX))
        Insertnumber(6);
    else if(IsKeyPressed(KEY_SEVEN))
        Insertnumber(7);
    else if(IsKeyPressed(KEY_EIGHT))
        Insertnumber(8);
    else if(IsKeyPressed(KEY_NINE))
        Insertnumber(9);
    else if(IsKeyPressed(KEY_BACKSPACE))
        Deletenumber();
}

#endif