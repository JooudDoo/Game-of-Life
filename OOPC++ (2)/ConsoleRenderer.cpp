#include "Console.h"
#include <iostream>
#include <conio.h>

#define WriteConsoleAtr(atr, cout, pos, written) WriteConsoleOutputAttribute(cnPref.cOut, atr, cout, pos, written)

GameRenderer::GameRenderer(const ConsolePreferences& consolePrefs) : focusMousePosition(topLeft), cnPref(consolePrefs), isNameRendered(false), isInstRendered(false), targetFPS(0), prevState(none), prevFPS(-1){}

void GameRenderer::prepConsole() {
    clearConsole();
    initCanvas();
    renderBorder();
}

void GameRenderer::clearWriteConsole(const SHORT& lines) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(cnPref.cOut, &csbi);
    COORD cursorPos = csbi.dwCursorPosition;
    SHORT consoleInY = cnPref.canvasStartPos.Y + cnPref.canvasHeight + 3;
    DWORD count;
    FillConsoleOutputCharacter(cnPref.cOut, (CHAR_T)' ', lines * cnPref.consoleMaxX, { 0, consoleInY }, &count);
    setCursorPos(cursorPos);
}

void GameRenderer::clearConsole() {
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
    GetConsoleScreenBufferInfo(cnPref.cOut, &screen);

    FillConsoleOutputCharacterA(
        cnPref.cOut, ' ', screen.dwSize.X * screen.dwSize.Y, focusMousePosition, &written
    );
    FillConsoleOutputAttribute(
        cnPref.cOut, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, focusMousePosition, &written
    );
    setCursorPos(focusMousePosition);
}

void GameRenderer::setUniverseName(const std::string& name) {
    universeName = name;
    isNameRendered = false;
}

void GameRenderer::setTargetFPS(const SHORT& newTargetFPS) {
    targetFPS = newTargetFPS;
}

void GameRenderer::renderFrame(const Frame& fr) {
    if (prevFrame.height == 0) {
        prevFrame = Frame(fr.width, fr.height);
    }
    if (fr.height > cnPref.canvasHeight || fr.width > cnPref.canvasWidth) {
        throw std::exception("Unsuitable frame size");
    }

    if (frameDiff(fr, prevFrame) > cnPref.canvasSquare/30) {
        renderFrameByLine(fr);
    }
    else {
        renderFrameByPix(fr);
    }
    setCursorPos(focusMousePosition);
    prevFrame = fr;
}

void GameRenderer::writeLineWithPrefix(const std::string& line, const char* prefix) {
    if (isIntMode) return;
    COUT << std::string(consoleWritePadding, ' ') + prefix;
    COUT << line;
    COUT << std::endl;
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
        setCursorPos(focusMousePosition);
    }
}

void GameRenderer::renderFrame(const Field& fr) {
    Frame frame = Frame(fr);
    renderFrame(frame);
}

void GameRenderer::renderGUI(const SHORT& currentFPS, const ConsoleInteractiveCode& state) {
    isIntMode = state == consoleMode ? false : true;
    renderName();
    renderInstruction();
    renderFPS(currentFPS);
    renderState(state);
    setCursorPos(focusMousePosition);
}

void GameRenderer::renderName() {
    if (isNameRendered)
        return;
    COORD cursorPointer = cnPref.menuPos;
    setCursorPos(cursorPointer);
    COUT << std::string(36, ' ');
    setCursorPos(cursorPointer);
    COUT << "  \"" << universeName << "\"";
    isNameRendered = true;
}
void GameRenderer::renderFPS(const SHORT& currentFPS) {
    if (prevFPS == currentFPS)
        return;
    COORD cursorPointer = { cnPref.menuPos.X, cnPref.menuPos.Y + 1};
    setCursorPos(cursorPointer);
    COUT << std::string(25, ' ');
    setCursorPos(cursorPointer);
    COUT << "FPS: " << currentFPS << " (target: " << targetFPS << ")";
    prevFPS = currentFPS;
}
void GameRenderer::renderState(const ConsoleInteractiveCode& state) {
    if (prevState == state)
        return;
    COORD cursorPointer = { cnPref.menuPos.X, cnPref.menuPos.Y + 2 };
    setCursorPos(cursorPointer);
    COUT << std::string(25, ' ');
    setCursorPos(cursorPointer);
    COUT << "State: " << consoleInterCodeToString(state);
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
    setCursorPos(cursorPointer);
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

void GameRenderer::setFocusMousePosition(const COORD& newPos) {
    focusMousePosition = newPos;
}

COORD GameRenderer::getCanvasSize() const {
    return { cnPref.canvasWidth, cnPref.canvasHeight };
}