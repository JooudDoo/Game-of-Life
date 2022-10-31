#pragma once
#include "utility.h"

class GameRenderer
{
public:
	GameRenderer();
	GameRenderer(const SHORT&, const SHORT&);
	~GameRenderer() = default;

	PlAction checkPlayer(const bool& useMouse);

	void renderFrame(Frame&);
	void renderFrame(Field&);
	void renderGUI(const SHORT&, const ConsoleCodes&);
	void renderBorder();

	void setUniverseName(const std::string&);
	void setTargetTPS(const SHORT&);

private:
	void InitConsole();
	void InitCanvas();
	void drawSymbol(const SHORT& x, const SHORT& y, CHAR_T sym);
	PlAction keyboardHandler(const KEY_EVENT_RECORD&);
	PlAction mouseHandler(const MOUSE_EVENT_RECORD&, const bool&);

	void renderName();
	void renderTPS(const SHORT& currentTPS);
	void renderState(const ConsoleCodes&);
	void renderInstruction();

	COORD menuPos;

	Frame prevFrame;
	HANDLE cInput;
	HANDLE cOut;
	SHORT consoleMaxX;
	SHORT consoleMaxY;

	SHORT canvasHeight;
	SHORT canvasWidth;
	COORD canvasStartPos;

	BOOL isNameRendered;
	BOOL isInstRendered;
	std::string universeName;
	SHORT targetTPS;
	ConsoleCodes prevState;
	SHORT prevTPS;
};