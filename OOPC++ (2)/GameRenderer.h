#pragma once
#include <windows.h>
#include "Game.h"
#include <io.h>
#include <vector>


class GameRenderer
{
public:
	GameRenderer();
	GameRenderer(const SHORT&, const SHORT&);
	~GameRenderer() = default;

	void renderFrame(Frame&);
	void renderFrame(Field&);
	void renderBorder();

private:
	void InitConsole();
	void InitCanvas();
	void drawSymbol(const SHORT& x, const SHORT& y, CHAR_T sym);

	Frame prevFrame;
	HANDLE cInput;
	HANDLE cOut;
	SHORT consoleMaxX;
	SHORT consoleMaxY;

	SHORT canvasHeight;
	SHORT canvasWidth;

	COORD canvasStartPos;
};