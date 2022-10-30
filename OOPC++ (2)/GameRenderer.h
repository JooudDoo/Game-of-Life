#pragma once
#include "utility.h"

class GameRenderer
{
public:
	GameRenderer();
	GameRenderer(const SHORT&, const SHORT&);
	~GameRenderer() = default;

	bool checkPlayer();

	void renderFrame(Frame&);
	void renderFrame(Field&);
	void renderBorder();

private:
	void InitConsole();
	void InitCanvas();
	void drawSymbol(const SHORT& x, const SHORT& y, CHAR_T sym);
	bool keyboardHandler(const KEY_EVENT_RECORD&);

	Frame prevFrame;
	HANDLE cInput;
	HANDLE cOut;
	SHORT consoleMaxX;
	SHORT consoleMaxY;

	SHORT canvasHeight;
	SHORT canvasWidth;

	COORD canvasStartPos;
};