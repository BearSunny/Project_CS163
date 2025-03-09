#ifndef DECLARE_H
#define DECLARE_H

#include <raylib.h>
#include <bits/stdc++.h>

using namespace std;

typedef enum GameScreen {MAINMENU, LINKEDLIST, HASH, TREE, GRAPH} GameScreen;

const int screenWidth = 1000;
const int screenHeight = 800;

bool InsertButton = false;
bool DeleteButton = false;
bool FindButton = false;
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

#endif
