﻿#include "Console.h"
#include <iostream>
#include <conio.h>

#define WriteConsoleAtr(atr, cout, pos, written) WriteConsoleOutputAttribute(cnPref.cOut, atr, cout, pos, written)

constexpr COORD topLeft = { 0, 0 };

GameRenderer::GameRenderer(const ConsolePreferences& consolePrefs) : cnPref(consolePrefs), isNameRendered(false), isInstRendered(false), targetTPS(0), prevState(none), prevTPS(-1){}

void GameRenderer::prepConsole() {
    cleanConsole();
    initCanvas();
    renderBorder();
}

void GameRenderer::cleanConsole() {
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(cnPref.cOut, &screen);

    FillConsoleOutputCharacterA(
        cnPref.cOut, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        cnPref.cOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    setCursorPos(topLeft);
}

void GameRenderer::setUniverseName(const std::string& name) {
    universeName = name;
    isNameRendered = false;
}

void GameRenderer::setTargetTPS(const SHORT& newTargetTPS) {
    targetTPS = newTargetTPS;
}

void GameRenderer::renderFrame(const Frame& fr) {
    if (prevFrame.height == 0) {
        prevFrame = Frame(fr.width, fr.height);
    }
    if (fr.height > cnPref.canvasHeight || fr.width > cnPref.canvasWidth) {
        throw std::exception("Unsuitable frame size");
    }
    if (frameDiff(fr, prevFrame) > cnPref.canvasSquare/10) {
        renderFrameByLine(fr);
    }
    else {
        renderFrameByPix(fr);
    }
    setCursorPos(topLeft);
    prevFrame = fr;
}

int64_t GameRenderer::frameDiff(const Frame& frNext, const Frame& frPrev) {
    int64_t diffPix = 0;
    for (SHORT line = 0; line < frNext.height; line++) {
        for (SHORT x = 0; x < frNext.width; x++) {
            diffPix += 1 * !!(frNext.at(line, x) - frPrev.at(line, x));
        }
    }
    return diffPix;
}

void GameRenderer::renderFrameByLine(const Frame& fr) {
    COORD cursorPosition = cnPref.canvasStartPos;
    for (SHORT line = 0; line < fr.height; line++) {
        cursorPosition.Y = line + cnPref.canvasStartPos.Y;
        setCursorPos(cursorPosition);
        COUT << fr.line(line);
    }
}

void GameRenderer::renderFrameByPix(const Frame& fr) {
    COORD cursorPosition = cnPref.canvasStartPos;
    for (SHORT line = 0; line < fr.height; line++) {
        cursorPosition.Y = line + cnPref.canvasStartPos.Y;
        for (SHORT x = 0; x < fr.width; x++) {
            if (prevFrame.at(line, x) != fr.at(line, x)) {
                cursorPosition.X = x + cnPref.canvasStartPos.X;
                setCursorPos(cursorPosition);
                COUT << fr.at(line, x);
            }
        }
        setCursorPos(topLeft);
    }
}

void GameRenderer::renderFrame(const Field& fr) {
    Frame frame = Frame(fr);
    renderFrame(frame);
}

void GameRenderer::renderGUI(const SHORT& currentTPS, const ConsoleCodes& state) {
    renderName();
    renderInstruction();
    renderTPS(currentTPS);
    renderState(state);
    setCursorPos(topLeft);
}

void GameRenderer::renderName() {
    if (isNameRendered)
        return;
    COORD cursorPointer = cnPref.menuPos;
    setCursorPos(cursorPointer);
    COUT << "  \"" << universeName << "\"";
    isNameRendered = true;
}
void GameRenderer::renderTPS(const SHORT& currentTPS) {
    if (prevTPS == currentTPS)
        return;
    COORD cursorPointer = { cnPref.menuPos.X, cnPref.menuPos.Y + 1};
    setCursorPos(cursorPointer);
    COUT << std::string(25, ' ');
    setCursorPos(cursorPointer);
    COUT << "TPS: " << currentTPS << " (target: " << targetTPS << ")";
    prevTPS = currentTPS;
}
void GameRenderer::renderState(const ConsoleCodes& state) {
    if (prevState == state)
        return;
    COORD cursorPointer = { cnPref.menuPos.X, cnPref.menuPos.Y + 2 };
    setCursorPos(cursorPointer);
    COUT << std::string(25, ' ');
    setCursorPos(cursorPointer);
    COUT << "State: " << consoleCodeToString(state);
    prevState = state;
}
void GameRenderer::renderInstruction() {
    if (isInstRendered)
        return;
    WORD text_atribute = COMMON_LVB_UNDERSCORE | COMMON_LVB_LEADING_BYTE | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    DWORD l;
    COORD cursorPointer = { cnPref.menuPos.X, cnPref.menuPos.Y + 5 };
    cursorPointer.X += 3;
    setCursorPos( cursorPointer);
    COUT << "Reset";
    WriteConsoleAtr(&text_atribute, 1, cursorPointer, &l);

    cursorPointer.Y += 1;
    setCursorPos( cursorPointer);
    COUT << "Pause/unPause";
    WriteConsoleAtr(&text_atribute, 1, cursorPointer, &l);
    cursorPointer.X += 8;
    WriteConsoleAtr(&text_atribute, 1, cursorPointer, &l);
    cursorPointer.X -= 8;

    cursorPointer.Y += 1;
    setCursorPos(cursorPointer);
    COUT << "SaveToReset";
    WriteConsoleAtr(&text_atribute, 1, cursorPointer, &l);

    cursorPointer.Y += 1;
    setCursorPos( cursorPointer);
    COUT << "Quit";
    WriteConsoleAtr(&text_atribute, 1, cursorPointer, &l);
    isInstRendered = true;
}

void GameRenderer::initCanvas() {
    for (SHORT x = cnPref.canvasStartPos.X; x < cnPref.canvasWidth + cnPref.canvasStartPos.X; x++) {
        for (SHORT y = cnPref.canvasStartPos.Y; y < cnPref.canvasHeight + cnPref.canvasStartPos.Y; y++) {
            drawSymbol(x, y, EMPTY_CELL);
        }
    }
}

void GameRenderer::renderBorder() {
    SHORT bWidth = cnPref.canvasWidth + cnPref.canvasStartPos.X;
    SHORT bHeight = cnPref.canvasHeight + cnPref.canvasStartPos.Y;
    drawSymbol(cnPref.canvasStartPos.X-1, cnPref.canvasStartPos.Y-1, BORDERCORNER);
    drawSymbol(bWidth, cnPref.canvasStartPos.Y-1, BORDERCORNER);
    drawSymbol(bWidth, bHeight, BORDERCORNER);
    drawSymbol(cnPref.canvasStartPos.X-1, bHeight, BORDERCORNER);
    
    SHORT y = cnPref.canvasStartPos.Y - 1;
    SHORT x = 0;
    for (x = cnPref.canvasStartPos.X; x < bWidth; x++) {
        drawSymbol(x, y, BORDERHORIZONTAL);
    }
    y = cnPref.canvasStartPos.Y + cnPref.canvasHeight;
    for (x = cnPref.canvasStartPos.X; x < bWidth; x++) {
        drawSymbol(x, y, BORDERHORIZONTAL);
    }
    x = cnPref.canvasStartPos.X - 1;
    for (y = cnPref.canvasStartPos.Y; y < bHeight; y++) {
        drawSymbol(x, y, BORDERVERTICAL);
    }
    x = cnPref.canvasStartPos.X + cnPref.canvasWidth;
    for (y = cnPref.canvasStartPos.Y; y < bHeight; y++) {
        drawSymbol(x, y, BORDERVERTICAL);
    }
}

bool GameRenderer::setCursorPos(const COORD& pos) {
    return SetConsoleCursorPosition(cnPref.cOut, pos);
}

void GameRenderer::drawSymbol(const SHORT& x,const SHORT& y, CHAR_T sym) {
    setCursorPos({x, y});
    COUT << sym;
}