#include "Console.h"
#include <iostream>
#include <conio.h>
#include <regex>

#define CTRLSTATE(x) (bool(x.dwControlKeyState & LEFT_CTRL_PRESSED))

GameInput::GameInput(const ConsolePreferences& consolePrefs) : cnPref(consolePrefs) {};

std::vector<PlAction> GameInput::checkPlayer(const bool& useMouse) {
    std::vector<PlAction> state = { };
    if (!_kbhit() && !useMouse)
        return state;
    INPUT_RECORD irInBuf[128];
    DWORD cNumRead;
    ReadConsoleInput(cnPref.cInput, irInBuf, 128, &cNumRead);
    for (DWORD i = 0; i < cNumRead; i++)
    {
        switch (irInBuf[i].EventType)
        {
        case KEY_EVENT:
            if (irInBuf[i].Event.KeyEvent.bKeyDown && irInBuf[i].Event.KeyEvent.wVirtualKeyCode != 17)
                state.push_back(keyboardHandler(irInBuf[i].Event.KeyEvent));
            break;
        case MOUSE_EVENT:
            state.push_back(mouseHandler(irInBuf[i].Event.MouseEvent, useMouse));
            break;
        }
    }
    return state;
}

/*
* pass some ticks
* exit
* switch target tps
* dump to file
* load from file
* switch name
* clear field
* help
*/
std::pair<ConsoleWriteCode, std::string> GameInput::proccesInput() {
    std::string command;
    COUT << std::string(consoleWritePadding, ' ') + consoleWriteUserInputPrefix;
    std::getline(std::cin, command);
    std::regex re("([^ ]*) ?(.*)");
    std::smatch parsedCommand;
    std::regex_search(command, parsedCommand, re);
    std::vector<std::string> params = { parsedCommand[1], parsedCommand[2]};

    if (params[1] == "") {
        params[1] = nullConsoleParametr;
    }

    for (auto codeAliases : aliasesForWriteCodes) {
            auto isFound = find(codeAliases.alias.begin(), codeAliases.alias.end(), params[0]);
        if (isFound != codeAliases.alias.end())
            return { codeAliases.code, params[1] };
    }

    return { NAC, ""};
}

PlAction GameInput::keyboardHandler(const KEY_EVENT_RECORD& mr) {
    int keyPressed = mr.uChar.AsciiChar;
    switch (keyPressed) {
    case('q'):
    case('Q'):
        return { quit, NULL, CTRLSTATE(mr) };
    case('p'):
    case('P'):
        return { pause, NULL, CTRLSTATE(mr) };
    case('r'):
    case('R'):
        return { reset, NULL, CTRLSTATE(mr) };
    case('s'):
    case('S'):
        return { saveFieldToReset, NULL, CTRLSTATE(mr) };
    case('c'):
    case('C'):
        return { consoleMode, NULL, CTRLSTATE(mr) };
    default:
        break;
    }
    return { none, NULL, CTRLSTATE(mr) };
}

PlAction GameInput::mouseHandler(const MOUSE_EVENT_RECORD& mr, const bool& useMouse) {
    if (!useMouse)
        return { none, NULL };
    switch (mr.dwEventFlags) {
    case 0:
        if (mr.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) {
            SHORT x = mr.dwMousePosition.X - cnPref.canvasStartPos.X;
            SHORT y = mr.dwMousePosition.Y - cnPref.canvasStartPos.Y;
            return { mouseClick, {x,y}, CTRLSTATE(mr) };
        }
        break;
    case MOUSE_MOVED:
        if (mr.dwControlKeyState & LEFT_CTRL_PRESSED) {
            SHORT x = mr.dwMousePosition.X - cnPref.canvasStartPos.X;
            SHORT y = mr.dwMousePosition.Y - cnPref.canvasStartPos.Y;
            return { mouseCtrlMove, {x,y}, CTRLSTATE(mr) };
        }
        break;
    default:
        break;
    }
    return { none, NULL, CTRLSTATE(mr) };
}
