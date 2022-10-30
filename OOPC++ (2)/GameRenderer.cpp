#include "GameRenderer.h"
#include <TCHAR.H>
#include <iostream>

GameRenderer::GameRenderer() : consoleMaxX(80), consoleMaxY(50) {
	InitConsole();
}

void GameRenderer::renderFrame(Frame& fr) {
    if (fr.height > consoleMaxY || fr.width > consoleMaxX) {
        throw std::exception("Unsuitable frame size");
    }
    COORD cursorPosition{ 0, 0 };
    for (SHORT line = 0; line < fr.height; line++) {
        cursorPosition.Y = line;
        SetConsoleCursorPosition(cOut, cursorPosition);
        std::wcout << fr.data[line];
    }
}

void GameRenderer::renderFrame(Field& fr) {
    Frame frame = Frame(fr);
    renderFrame(frame);
}

void GameRenderer::InitConsole() {
    cOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cInput = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleTitle(TEXT("Life game"));
    SetConsoleOutputCP(CP_UTF8);

    SMALL_RECT console_position { 0, 0, consoleMaxX, consoleMaxY };
    SetConsoleWindowInfo(cOut, TRUE, &console_position);
    SetConsoleTextAttribute(cOut, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    DWORD fdwMode = ENABLE_WINDOW_INPUT;
    SetConsoleMode(cInput, fdwMode);
}