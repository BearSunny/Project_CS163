#include "../header/GraphButton.h"

Button::Button(Rectangle rect, const std::string text, float yText, Color textColor, float fontSize, Font font) {
    this->outerRect = rect;
    this->content = text;
    this->font = font;
    this->fontSize = fontSize;
    this->contentColor = textColor;
    Vector2 textSize = MeasureTextEx(this->font, text.c_str(), this->fontSize,2);
    this->contentPos = {this->outerRect.x + (this->outerRect.width-textSize.x)/2, (yText == -1) ? (rect.y + (rect.height - textSize.y)/2) : yText};
    this->isHovered = false;
}

void Button::draw(float radius) {
    this->isHovered = CheckCollisionPointRec(GetMousePosition(), this->outerRect);
    DrawRectangleRounded(this->outerRect, radius/55, 32, this->isHovered ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawTextPro(this->font, this->content.c_str(), this->contentPos, ORIGIN, 0, this->fontSize, 2, this->contentColor);
}

int Button::handle() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && this->isHovered) {
        this->isHovered = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    return 0;
}

TittleButton::TittleButton(Rectangle rect, const std::string text, float yText, Color textColor, float fontSize, Font font) {
    this->outerRect = rect;
    this->content = text;
    this->fontSize = fontSize;
    this->font = font;
    this->contentColor = textColor;
    Vector2 textSize = MeasureTextEx(this->font, text.c_str(), this->fontSize,2);
    this->contentPos = {this->outerRect.x + (this->outerRect.width-textSize.x)/2, (yText == -1) ? (rect.y + (rect.height - textSize.y)/2) : yText};
    this->isHovered = false;
    this->isHoveredBackPage = false;
    this->backPage = BACK_PAGE;
    this->backPageRadius = 20;
    this->backPagePos = {432,54};
}

void TittleButton::draw(float radius) {
    this->isHovered = CheckCollisionPointRec(GetMousePosition(), this->outerRect);
    DrawRectangleRounded(this->outerRect, radius/55, 32, this->isHovered ? Color{234,119,119,255} : Color{249,208,208,255});
    DrawTextPro(this->font, this->content.c_str(), this->contentPos, ORIGIN, 0, this->fontSize, 2, this->contentColor);
    this->isHoveredBackPage = CheckCollisionPointCircle(GetMousePosition(), this->backPagePos
                                                        , this->backPageRadius);
    DrawCircleV(this->backPagePos, this->backPageRadius, this->isHoveredBackPage ? Color{234,119,119,255} : Color{249,208,208,255});
    drawPicture(this->backPage, {421, 43,22,22}, 0.0f, ORIGIN, WHITE);
}

int TittleButton::handle() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && this->isHoveredBackPage) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 1;
    }
    return 0;
}

ButtonImage::ButtonImage(Rectangle outerRect, Rectangle innerRect, const char* path, const std::string text, float yText, Color textColor, float fontSize, Font font) {
    this->outerRect = outerRect;
    this->innerRect = innerRect;
    this->texture = LoadTexture(path);
    this->content = text;
    this->contentPos = {this->outerRect.x + (this->outerRect.width - MeasureTextEx(font, text.c_str(), fontSize, 5).x)/2, yText};
    this->contentColor = textColor;
    this->fontSize = fontSize;
    this->font = font;
    
}

void ButtonImage::draw(float radius) {
    this->isHovered = CheckCollisionPointRec(GetMousePosition(), this->outerRect);
    DrawRectangleRounded(outerRect, radius/100, 32,this->isHovered ? THEME.HOVER_MENU : THEME.MENU_BUTTON);
    drawPicture(this->texture, this->innerRect, 0.0f, ORIGIN, WHITE);
    DrawTextPro(this->font, this->content.c_str(), this->contentPos, ORIGIN, 0, this->fontSize, 2, this->contentColor);
}

void InputStr::draw() {
    DrawRectangleRounded(this->textBox, 0.2f, 64, {229,247,255,255});
    DrawRectangleRoundedLinesEx(this->textBox, 0.2f, 64, 3, mouseOnText ? BLACK : GRAY);
    Vector2 size = MeasureTextEx(this->font, this->text.c_str(), this->fontSize, 2);
    if (this->text.empty() && !this->mouseOnText) {
        DrawTextEx(this->font, this->placeHolder.c_str(), {this->textBox.x + 16 , this->textBox.y + float(this->textBox.height - size.y)/2}, this->fontSize, 2, BLACK);
    }
    else {
        DrawTextEx(this->font, this->text.c_str(), {this->textBox.x + 16 , this->textBox.y + float(this->textBox.height - size.y)/2}, this->fontSize, 2, BLACK);
    }
    
    if (this->mouseOnText) {
        if (this->text.length() < 30) {
            if (((this->framesCounter/20) % 2) == 0) {
                DrawTextEx(this->font, "|", {this->textBox.x + 16 + size.x, this->textBox.y + float(this->textBox.height - size.y)/2}, this->fontSize, 2, BLACK);
            }
        }
    }
}

void InputStr::resetText() {
    srand((int)time(0));
    this->changePlaceHolder(TextFormat("%d", rand() % 100));
}

void InputStr::update() {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(GetMousePosition(), this->textBox)) {
            this->mouseOnText = true;
        } else {
            this->mouseOnText = false;
        }
    }

    if (this->mouseOnText) {
        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && this->text.length() < 30) {
                this->text += (char)key;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            if (!this->text.empty()) {
                this->text.pop_back();
            }
        }
    }

    this->framesCounter++;
}

void InputStr::changePlaceHolder(const std::string newPlaceHolder) {
    this->placeHolder = newPlaceHolder;
}

std::string InputStr::getText() {
    return this->text;
}

void CircleButton::draw() {
    DrawCircleV(this->center, this->radius, this->isHovered ? THEME.HOVER_BUTTON : THEME.BUTTON);
    drawPicture(this->texture, {center.x - radius + 2, center.y - radius +2, 2*radius -4, 2*radius -4},  0.0f, ORIGIN, WHITE);
}

void CircleButton::handle() {
    this->isHovered = CheckCollisionPointCircle(GetMousePosition(), this->center, this->radius);
}

void CircleButton::changeTexture(const char* file) {
    UnloadTexture(this->texture);
    this->texture = LoadTexture(file);
}

void ProgressBar::draw(int type) {
    DrawCircle(33.5, 643.5, 17.5, CheckCollisionPointCircle(GetMousePosition(), {33.5, 643.5}, 17.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawCircle(72.5, 643.5, 17.5 , CheckCollisionPointCircle(GetMousePosition(), {72.5, 643.5}, 17.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawCircle(121.77, 643.5, 25.5 , CheckCollisionPointCircle(GetMousePosition(), {121.77, 643.5}, 25.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawCircle(168.5, 643.5, 17.5 , CheckCollisionPointCircle(GetMousePosition(), {168.5, 643.5}, 17.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawCircle(207.5, 643.5, 17.5 , CheckCollisionPointCircle(GetMousePosition(), {207.5, 643.5}, 17.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawCircle(263.5, 610.5, 12.5 , CheckCollisionPointCircle(GetMousePosition(), {263.5, 610.5}, 12.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    DrawCircle(263.5, 670.5, 12.5 , CheckCollisionPointCircle(GetMousePosition(), {263.5, 670.5}, 12.5) ? THEME.HOVER_BUTTON : THEME.BUTTON);
    
    DrawTextPro(font, TextFormat("%.1fx", this->speed), {246, 631}, ORIGIN, 0.f, 20, 1, BLACK);
    
    drawPicture(DOUBLE_BACK, {19.5, 629, 28, 28},  0.0f, ORIGIN, WHITE);
    drawPicture(BACK, {56.75, 629, 28, 28},  0.0f, ORIGIN, WHITE);
    if (type == 0) drawPicture(PAUSE, {110, 629, 25, 25},  0.0f, ORIGIN, WHITE);
    else if (type == 1) drawPicture(PLAY, {110, 629, 25, 25},  0.0f, ORIGIN, WHITE);
    else drawPicture(REPLAY, {110, 629, 25, 25},  0.0f, ORIGIN, WHITE);
    drawPicture(NEXT, {155, 629, 28, 28},  0.0f, ORIGIN, WHITE);
    drawPicture(DOUBLE_NEXT, {194, 629, 28, 28},  0.0f, ORIGIN, WHITE);
    drawPicture(SPEED_UP, {250.5, 598, 25, 25},  0.0f, ORIGIN, WHITE);
    drawPicture(SPEED_DOWN, {250.5, 658, 25, 25},  0.0f, ORIGIN, WHITE);

    //DrawRectangleRec({4.02, 589, 297, 6}, NONHOVERED_BUTTON_LIGHT_THEME);
    DrawRectangleRec({0, 590, this->maxStep != 0 ? this->curStep*297.f/this->maxStep : 0, 4}, {249,208,208,255});
}

void ProgressBar::updateMaxStep(int max) {
    this->maxStep = max;
    this->curStep = 0;
}

void ProgressBar::updateSpeed(float speed) {
    this->speed = speed;
}

//default -2 for skip back, 2 for skip next, else: back, next(-1,1)
void ProgressBar::updateStep(int step) {
    if (step == -2) this->curStep = 0;
    else if (step == 2) this->curStep = maxStep;
    else this->curStep += step;
}

int ProgressBar::handle() {
    int flag = 10;
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointCircle(GetMousePosition(), {33.5, 643.5}, 17.5)) {
            flag = -2;
        }
        if (CheckCollisionPointCircle(GetMousePosition(), {72.5, 643.5}, 17.5)) {
            flag = -1;
        }
        if (CheckCollisionPointCircle(GetMousePosition(), {121.77, 643.5}, 25.5)) {
            flag = 0;
        }
        if (CheckCollisionPointCircle(GetMousePosition(), {168.5, 643.5}, 17.5)) {
            flag = 1;
        }
        if (CheckCollisionPointCircle(GetMousePosition(), {207.5, 643.5}, 17.5)) {
            flag = 2;
        }
        if (CheckCollisionPointCircle(GetMousePosition(), {263.5, 610.5}, 12.5)) {
            this->speed += (this->speed < 2) ? 0.1 : 0;
            flag = 3;
        }
        if (CheckCollisionPointCircle(GetMousePosition(), {263.5, 670.5}, 12.5)) {
            this->speed -= (this->speed > 0.5) ? 0.1 : 0;
            flag = -3;
        }
    }
    if (flag != 10) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return flag;
}



void DrawTextInRect(Font font, const char *text, Rectangle rect, int fontSize, Color textColor) {
    int maxWidth = (int)rect.width;
    int textHeight = fontSize;
    char lineBuffer[1024];
    const char *currentChar = text;
    int lineCount = 0;

    while (currentChar && *currentChar) {
        int lineWidth = 0;
        int charsToFit = 0;
        while (*currentChar && (lineWidth < maxWidth)) {
            lineBuffer[charsToFit++] = *currentChar;
            lineBuffer[charsToFit] = '\0'; 

            lineWidth = MeasureTextEx(font, lineBuffer, fontSize, 2).x;
            currentChar++;

            if (lineWidth > maxWidth) {
                currentChar--;
                lineBuffer[--charsToFit] = '\0';
                break;
            }
        }
        Vector2 textPosition;
        textPosition.x = rect.x;
        textPosition.y = rect.y + lineCount * textHeight;
        DrawTextEx(font, lineBuffer, textPosition, fontSize, 2, textColor);

        lineCount++;

        while (*currentChar && (*currentChar == ' ' || *currentChar == '\n')) {
            currentChar++;
        }
        if ((rect.y + lineCount * textHeight) >= (rect.y + rect.height)) break;
    }
}

void drawInfor(const std::string infor, Font font) {
    DrawTextInRect(font, infor.c_str(), {20, 335, 260, 65}, CODE_SIZE, THEME.INFOR);
}

void drawCode(std::string code, size_t n ,std::vector<int> highlight, Font font) {
    float lineHeight = (float)CODE_SIZE + 0.7;
    Vector2 CodeOffset = {20, 130};
    
    for (size_t i = 0; i<n; i++) {
        bool flag = false;
        for (auto line : highlight) {
            if (i == line) {
                flag = true;
                break;
            }
        }
        Color color = flag ? THEME.HIGHLIGHT_TEXT : Color {0,0,0,0};
        DrawRectangle(CodeOffset.x, CodeOffset.y + i * lineHeight, 260, lineHeight, color);
    }
    DrawTextEx(font, code.c_str(), {CodeOffset.x, CodeOffset.y + 1}, CODE_SIZE, 2, BLACK);
}

void drawSideBar(int type, std::string code, std::vector<int> lines, std::string infor, ProgressBar bar, Font font) {
    DrawRectangleRounded({0, 110, 297, 584}, 0.10f, 32, THEME.SIDEBAR);
    DrawLineEx({0, 326}, {297, 326}, 1.3, THEME.SEPERATOR);
    DrawLineEx({0, 408}, {297, 408}, 1.3, THEME.SEPERATOR);
    DrawLineEx({126, 408}, {126, 592}, 1.3, THEME.SEPERATOR);
    DrawLineEx({0, 592}, {297, 592}, 1.3, THEME.SEPERATOR);
    drawCode(code, 8, lines, font);
    drawInfor(infor, font);
    bar.draw(type);
}
