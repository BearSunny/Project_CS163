#include "../header/declare.h"

// Global variable definitions
Texture2D PLAY;
Texture2D PAUSE;
Texture2D REPLAY;
Texture2D NEXT;
Texture2D DOUBLE_NEXT;
Texture2D BACK;
Texture2D DOUBLE_BACK;
Texture2D SPEED_UP;
Texture2D SPEED_DOWN;
Texture2D BACK_PAGE;
theme THEME;
Font FONT;

const int screenWidth = 1600;
const int screenHeight = 900;
const Vector2 ORIGIN = {0.0f, 0.0f};
const float CODE_SIZE = 15;

GameScreen currentScreen = MAINMENU;
bool InsertButton = false, DeleteButton = false, FindButton = false;
int FindQuerry = -1, FindNumber = 0, framecount = 0, length = 0;
char number[10] = {};

// Function implementations

void initResource() 
{
    PLAY = LoadTexture("./asset/Play.png");
    PAUSE = LoadTexture("./asset/Pause.png");
    REPLAY = LoadTexture("./asset/Replay.png");
    BACK = LoadTexture("./asset/Chevron left.png");
    DOUBLE_BACK = LoadTexture("./asset/Chevrons left.png");
    NEXT = LoadTexture("./asset/Chevron right.png");
    DOUBLE_NEXT = LoadTexture("./asset/Chevrons right.png");
    SPEED_UP = LoadTexture("./asset/Chevron up.png");
    SPEED_DOWN = LoadTexture("./asset/Chevron down.png");
    BACK_PAGE = LoadTexture("./asset/BackPage.png");
    
    //Color
    THEME.BACKGROUND = {229,247,255, 255};
    THEME.MENU_BUTTON = {255,255,255,255};
    THEME.HOVER_MENU = {197,252,245,255};
    THEME.BUTTON = {229,247,255, 255};
    THEME.HOVER_BUTTON = {125,215,255, 255};
    THEME.SEPERATOR = {3, 178, 255, 255};
    THEME.INFOR = {6, 155, 91, 255};
    THEME.HIGHLIGHT_TEXT = {157, 238, 238, 255};
    THEME.HIGHLIGHT_NODE_1 = {55,221,149,255};
    THEME.HIGHLIGHT_NODE_2 = Fade(BLUE,60);
    THEME.NODE = {249,208,208,255};
    THEME.LINE = {182,157,157, 255};
    THEME.SIDEBAR = {250, 254, 255, 255};
    THEME.WEIGHT = {6, 155, 91, 255};
    //FONT = LoadFont("./Font/Roboto-Regular.ttf");
    FONT = LoadFontEx("./Font/SF-Pro-Display-Regular.otf", 40, 0, 255);
    
}

void Insertnumber(int x) {
    if (length == 4) return;
    number[length] = x + '0';
    ++length;
    number[length] = 0;
}

void Deletenumber() {
    if (length == 0) return;
    --length;
    number[length] = 0;
}

void DrawNumber(int data, int x, int y, int fs) {
    if (data == 0) {
        DrawText("0", x - MeasureText("0", fs) / 2, y - fs / 2, fs, BLACK);
        return;
    }
    string c;
    while (data) {
        c += data % 10 + '0';
        data /= 10;
    }
    reverse(c.begin(), c.end());
    DrawText(c.c_str(), x - MeasureText(c.c_str(), fs) / 2, y - fs / 2, fs, BLACK);
}

bool MouseButtonPressed(float x, float y, float u, float v) {
    Vector2 p = GetMousePosition();
    return (p.x >= x && p.x < u && p.y >= y && p.y < v);
}

void Updatenumber() {
    if (IsKeyPressed(KEY_ZERO))
        Insertnumber(0);
    else if (IsKeyPressed(KEY_ONE))
        Insertnumber(1);
    else if (IsKeyPressed(KEY_TWO))
        Insertnumber(2);
    else if (IsKeyPressed(KEY_THREE))
        Insertnumber(3);
    else if (IsKeyPressed(KEY_FOUR))
        Insertnumber(4);
    else if (IsKeyPressed(KEY_FIVE))
        Insertnumber(5);
    else if (IsKeyPressed(KEY_SIX))
        Insertnumber(6);
    else if (IsKeyPressed(KEY_SEVEN))
        Insertnumber(7);
    else if (IsKeyPressed(KEY_EIGHT))
        Insertnumber(8);
    else if (IsKeyPressed(KEY_NINE))
        Insertnumber(9);
    else if (IsKeyPressed(KEY_BACKSPACE))
        Deletenumber();
}

void Updatenumbercopy() {
    const char* n = GetClipboardText();
    for(int i = 0; n[i] != '\0'; ++i)
        if(n[i] < '0' || n[i] > '9')
            return;
    for(int i = 0; n[i] != '\0'; ++i)
        Insertnumber(n[i] - '0');
}

void drawPicture(const char* path, Rectangle desRec, float rotation, Vector2 origin, Color color) {
    Texture2D texture = LoadTexture(path);
    if (texture.id == 0) return;
    Rectangle srcRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, srcRect, desRec, origin, rotation, color);
    UnloadTexture(texture);
}

void drawPicture(Texture2D texture, Rectangle desRec, float rotation, Vector2 origin, Color color) {
    if (texture.id == 0) return;
    Rectangle srcRect = {0.0f, 0.0f, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, srcRect, desRec, origin, rotation, color);
}
