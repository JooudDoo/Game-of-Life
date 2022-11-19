#include "Console.h"
#include <iostream>
#include <conio.h>

inline long long time_to_msec(const sys_time_t& t) {
    return t.time * 1000LL + t.millitm;
}

Console::Console() {
    cnPref.canvasHeight = HEIGHTDEFAULT;
    cnPref.canvasWidth = WIDTHDEFAULT;
    InitConsole();
    isSetted = true;
    gRen = GameRenderer(cnPref);
    gInp = GameInput(cnPref);
    gRen.prepConsole();
}

Console::Console(const SHORT& cnWidth, const SHORT& cnHeight) {
    cnPref.canvasWidth = cnWidth;
    cnPref.canvasHeight = cnHeight;
    InitConsole();
    isSetted = true;
    gRen = GameRenderer(cnPref);
    gInp = GameInput(cnPref);
    gRen.prepConsole();
}

std::vector<PlAction> Console::getPlayerActions(const bool& useMouse) {
    return gInp.checkPlayer(useMouse);
}

void Console::renderConsoleFrame(const Frame& fr, const ConsoleInteractiveCode& state) {
    gRen.renderGUI(currentFPS, state);
    gRen.renderFrame(fr);
    int frameMeasure = gRen.targetFPS / 10 + 1;
    if (frameCounter >= frameMeasure) {
        currentFPS = calculateFPS();
        T_st = T_end;
        _ftime_s(&T_end);
        frameCounter = 0;
    }
    frameCounter++;
}

SHORT Console::calculateFPS() {
    double frameMeasure = gRen.targetFPS / 10 + 1;
    double milsBetFrame = (time_to_msec(T_end) - time_to_msec(T_st)) / frameMeasure;
    return 1000 / (milsBetFrame + !milsBetFrame);
}

void Console::switchToConsoleMode() {
    DWORD fdwMode = ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT | ENABLE_PROCESSED_OUTPUT | ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(cnPref.cInput, fdwMode);
    SHORT consoleInY = cnPref.canvasStartPos.Y + cnPref.canvasHeight + 3 + lastConsoleLine;
    gRen.setCursorPos({ 0, consoleInY });
    gRen.setFocusMousePosition({ 0, consoleInY });
}

void Console::switchFromConsoleMode() {
    DWORD fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(cnPref.cInput, fdwMode);
    gRen.setCursorPos(topLeft);
    gRen.setFocusMousePosition(topLeft);
}

void Console::consoleWriteProcessed() {
    lastConsoleLine += 1;
}

void Console::clearWriteConsole() {
    gRen.clearWriteConsole(lastConsoleLine);
    lastConsoleLine = 0;
    switchToConsoleMode();
}

void Console::writeLineWithPrefix(const std::string& line, const char* prefix) {
    gRen.writeLineWithPrefix(line, prefix);
    consoleWriteProcessed();
}

void Console::InitConsole() {
    cnPref.cOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cnPref.cInput = GetStdHandle(STD_INPUT_HANDLE);

    if (cnPref.canvasHeight > 50)
        std::cerr << "It's not recomended to create game frame with height more than 50";

    cnPref.canvasSquare = cnPref.canvasHeight * cnPref.canvasWidth;
    SetConsoleTitle(TEXT("Life game"));
    SetConsoleOutputCP(CP);
    DWORD fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT & ~ENABLE_QUICK_EDIT_MODE;
    SetConsoleMode(cnPref.cInput, fdwMode);
    cnPref.canvasStartPos = { 6, 3 };
    cnPref.menuPos = { cnPref.canvasWidth,  cnPref.canvasStartPos.Y };
    cnPref.menuPos.X += cnPref.canvasStartPos.X + 5;

    CONSOLE_FONT_INFO fontInfo;
    CONSOLE_FONT_INFOEX fontInfoE;
    GetCurrentConsoleFont(cnPref.cOut, TRUE, &fontInfo);
    COORD fontSize = GetConsoleFontSize(cnPref.cOut, fontInfo.nFont);

    fontInfoE.cbSize = sizeof(fontInfoE);
    GetCurrentConsoleFontEx(cnPref.cOut, FALSE, &fontInfoE);
    fontInfoE.dwFontSize.X = 12;
    fontInfoE.dwFontSize.Y = 16;
    fontInfoE.FontFamily = 48;
    fontInfoE.FontWeight = 400;
    fontInfoE.nFont = 0;
    wcscpy_s(fontInfoE.FaceName, L"Terminal");
    SetCurrentConsoleFontEx(cnPref.cOut, FALSE, &fontInfoE);


    cnPref.consoleMaxX = (cnPref.menuPos.X + 35) * fontInfoE.dwFontSize.X;
    cnPref.consoleMaxY = (cnPref.canvasHeight + cnPref.canvasStartPos.Y + 5) * fontInfoE.dwFontSize.Y;

    HWND hWindowConsole = GetConsoleWindow();
    RECT r;
    GetWindowRect(hWindowConsole, &r);
    MoveWindow(hWindowConsole, r.left, r.top, cnPref.consoleMaxX, cnPref.consoleMaxY, TRUE);
    SetConsoleTextAttribute(cnPref.cOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}
