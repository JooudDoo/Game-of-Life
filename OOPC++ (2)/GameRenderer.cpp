#include "GameRenderer.h"
#include <TCHAR.H>
#include <iostream>

GameRenderer::GameRenderer() : consoleMaxX(80), consoleMaxY(50), canvasHeight(50), canvasWidth(50) {
	InitConsole();
    InitCanvas();
    renderBorder();
}

GameRenderer::GameRenderer(const SHORT& iWidth, const SHORT& iHeight) : consoleMaxX(80), consoleMaxY(50), canvasHeight(iHeight), canvasWidth(iWidth) {
    InitConsole();
    InitCanvas();
    renderBorder();
}

void GameRenderer::renderFrame(Frame& fr) {
    if (prevFrame.height == 0) {
        prevFrame = Frame(fr.width, fr.height);
    }
    if (fr.height > canvasHeight || fr.width > canvasWidth) {
        throw std::exception("Unsuitable frame size");
    }
    COORD cursorPosition = canvasStartPos;
    for (SHORT line = 0; line < fr.height; line++) {
        cursorPosition.Y = line + canvasStartPos.Y;
        for (SHORT x = 0; x < fr.width; x++) {
            if (prevFrame.at(line, x) != fr.at(line, x)) {
                cursorPosition.X = x + canvasStartPos.X;
                SetConsoleCursorPosition(cOut, cursorPosition);
                COUT << fr.at(line, x);
            }
        }
        SetConsoleCursorPosition(cOut, {fr.width, fr.height});
    }
    prevFrame = fr;
}

void GameRenderer::renderFrame(Field& fr) {
    Frame frame = Frame(fr);
    renderFrame(frame);
}

void GameRenderer::InitConsole() {
    cOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleTitle(TEXT("Life game"));
    SetConsoleOutputCP(CP);

    SMALL_RECT console_position { 0, 0, consoleMaxX, consoleMaxY };
    SetConsoleWindowInfo(cOut, TRUE, &console_position);
    SetConsoleTextAttribute(cOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    DWORD fdwMode = ENABLE_WINDOW_INPUT;
    SetConsoleMode(cInput, fdwMode);
    canvasStartPos = { 6, 3 };
}

void GameRenderer::InitCanvas() {
    for (SHORT x = canvasStartPos.X; x < canvasWidth + canvasStartPos.X; x++) {
        for (SHORT y = canvasStartPos.Y; y < canvasHeight + canvasStartPos.Y; y++) {
            drawSymbol(x, y, EMPTY_CELL);
        }
    }
}

void GameRenderer::renderBorder() {
    SHORT bWidth = canvasWidth + canvasStartPos.X;
    SHORT bHeight = canvasHeight + canvasStartPos.Y;
    drawSymbol(canvasStartPos.X-1, canvasStartPos.Y-1, BORDERCORNER);
    drawSymbol(bWidth, canvasStartPos.Y-1, BORDERCORNER);
    drawSymbol(bWidth, bHeight, BORDERCORNER);
    drawSymbol(canvasStartPos.X-1, bHeight, BORDERCORNER);
    
    SHORT y = canvasStartPos.Y - 1;
    SHORT x = 0;
    for (x = canvasStartPos.X; x < bWidth; x++) {
        drawSymbol(x, y, BORDERHORIZONTAL);
    }
    y = canvasStartPos.Y + canvasHeight;
    for (x = canvasStartPos.X; x < bWidth; x++) {
        drawSymbol(x, y, BORDERHORIZONTAL);
    }
    x = canvasStartPos.X - 1;
    for (y = canvasStartPos.Y; y < bHeight; y++) {
        drawSymbol(x, y, BORDERVERTICAL);
    }
    x = canvasStartPos.X + canvasWidth;
    for (y = canvasStartPos.Y; y < bHeight; y++) {
        drawSymbol(x, y, BORDERVERTICAL);
    }
}

void GameRenderer::drawSymbol(const SHORT& x,const SHORT& y, CHAR_T sym) {
    SetConsoleCursorPosition(cOut, {x, y});
    COUT << sym;
}