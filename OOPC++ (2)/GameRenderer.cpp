#include "GameRenderer.h"
#include <iostream>
#include <conio.h>

GameRenderer::GameRenderer() : canvasHeight(HEIGHTDEFAULT), canvasWidth(WIDTHDEFAULT) {
	InitConsole();
    InitCanvas();
    renderBorder();
}

GameRenderer::GameRenderer(const SHORT& iWidth, const SHORT& iHeight) : canvasHeight(iHeight), canvasWidth(iWidth) {
    InitConsole();
    InitCanvas();
    renderBorder();
}

void GameRenderer::setUniverseName(const std::string& name) {
    universeName = name;
}

void GameRenderer::setTargetTPS(const SHORT& targetTPS) {
    GameRenderer::targetTPS = targetTPS;
}

PlAction GameRenderer::checkPlayer(const bool& useMouse) {
    PlAction state = {none, NULL};
    if (!_kbhit() && !useMouse) 
        return state;
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;
    ReadConsoleInput(cInput, irInBuf, 128, &cNumRead);
    for (DWORD i = 0; i < cNumRead; i++)
    {
        switch (irInBuf[i].EventType)
        {
        case KEY_EVENT:
            if(irInBuf[i].Event.KeyEvent.bKeyDown)
                state = keyboardHandler(irInBuf[i].Event.KeyEvent);
            break;

        case MOUSE_EVENT:
            state = mouseHandler(irInBuf[i].Event.MouseEvent, useMouse);
            break;
        }
    }
    return state;
}

PlAction GameRenderer::keyboardHandler(const KEY_EVENT_RECORD& mr) {
    int keyPressed = mr.uChar.AsciiChar;
    switch (keyPressed) {
    case('q'):
    case('Q'):
        return {quit, NULL};
    case('p'):
    case('P'):
        return {pause, NULL};
    case('r'):
    case('R'):
        return { reset, NULL };
    default:
        break;
    }
    return {none, NULL};
}

PlAction GameRenderer::mouseHandler(const MOUSE_EVENT_RECORD& mr, const bool& useMouse) {
    if (!useMouse)
        return {none, NULL};
    switch (mr.dwEventFlags) {
    case 0:
        if (mr.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            SHORT x = mr.dwMousePosition.X - canvasStartPos.X;
            SHORT y = mr.dwMousePosition.Y - canvasStartPos.Y;
            return { mouseClick, {x,y} };
        }
    default:
        break;
    }
    return { none, NULL };
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
        SetConsoleCursorPosition(cOut, {0, 0});
    }
    prevFrame = fr;
}

void GameRenderer::renderFrame(Field& fr) {
    Frame frame = Frame(fr);
    renderFrame(frame);
}

void GameRenderer::renderGUI(const SHORT& currentTPS, const ConsoleCodes& state) {

    renderName();
    renderInstruction();
    renderTPS(currentTPS);
    renderState(state);
    SetConsoleCursorPosition(cOut, { 0, 0 });
}

void GameRenderer::renderName() {
    if (isNameRendered)
        return;
    COORD cursorPointer = menuPos;
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << "  \"" << universeName << "\"";
    isNameRendered = true;
}
void GameRenderer::renderTPS(const SHORT& currentTPS) {
    if (prevTPS == currentTPS)
        return;
    COORD cursorPointer = {menuPos.X, menuPos.Y + 1};
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << std::string(25, ' ');
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << "TPS: " << currentTPS << " (target: " << targetTPS << ")";
    prevTPS = currentTPS;
}
void GameRenderer::renderState(const ConsoleCodes& state) {
    if (prevState == state)
        return;
    COORD cursorPointer = { menuPos.X, menuPos.Y + 2 };
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << std::string(25, ' ');
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << "State: " << consoleCodeToString(state);
    prevState = state;
}
void GameRenderer::renderInstruction() {
    if (isInstRendered)
        return;
    WORD text_atribute = COMMON_LVB_UNDERSCORE | COMMON_LVB_LEADING_BYTE | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    DWORD l;
    COORD cursorPointer = { menuPos.X, menuPos.Y + 5 };
    cursorPointer.X += 3;
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << "Reset";
    WriteConsoleOutputAttribute(cOut, &text_atribute, 1, cursorPointer, &l);
    cursorPointer.Y += 1;
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << "Pause/unPause";
    WriteConsoleOutputAttribute(cOut, &text_atribute, 1, cursorPointer, &l);
    cursorPointer.X += 8;
    WriteConsoleOutputAttribute(cOut, &text_atribute, 1, cursorPointer, &l);
    cursorPointer.Y += 1;
    cursorPointer.X -= 8;
    SetConsoleCursorPosition(cOut, cursorPointer);
    COUT << "Quit";
    WriteConsoleOutputAttribute(cOut, &text_atribute, 1, cursorPointer, &l);
    isInstRendered = true;
}

void GameRenderer::InitConsole() {
    cOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cInput = GetStdHandle(STD_INPUT_HANDLE);

    if (canvasHeight > 50)
        std::cerr << "It's not recomended to create game frame with height more than 50";

    SetConsoleTitle(TEXT("Life game"));
    SetConsoleOutputCP(CP);
    DWORD fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    SetConsoleMode(cInput, fdwMode);
    isNameRendered = false;
    isInstRendered = false;
    canvasStartPos = { 6, 3 };
    menuPos = { canvasWidth,  canvasStartPos.Y };
    menuPos.X += canvasStartPos.X + 5;

    consoleMaxX = (menuPos.X + 35)*8;
    consoleMaxY = (canvasHeight + canvasStartPos.Y + 5)*14;

    HWND hWindowConsole = GetConsoleWindow();
    RECT r;
    GetWindowRect(hWindowConsole, &r);
    MoveWindow(hWindowConsole, r.left, r.top, consoleMaxX, consoleMaxY, TRUE);
    SetConsoleTextAttribute(cOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
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