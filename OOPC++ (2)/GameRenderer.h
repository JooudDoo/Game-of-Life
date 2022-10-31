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
	void renderBorder();

private:
	void InitConsole();
	void InitCanvas();
	void drawSymbol(const SHORT& x, const SHORT& y, CHAR_T sym);
	PlAction keyboardHandler(const KEY_EVENT_RECORD&);
	PlAction mouseHandler(const MOUSE_EVENT_RECORD&, const bool&);

	Frame prevFrame;
	HANDLE cInput;
	HANDLE cOut;
	SHORT consoleMaxX;
	SHORT consoleMaxY;

	SHORT canvasHeight;
	SHORT canvasWidth;

	COORD canvasStartPos;
};