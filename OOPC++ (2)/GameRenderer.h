#pragma once
#include <windows.h>
#include "Game.h"
#include <vector>

class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer() = default;

	void renderFrame(Frame&);
	void renderFrame(Field&);

private:
	void InitConsole();

	HANDLE cInput;
	HANDLE cOut;
	SHORT consoleMaxX;
	SHORT consoleMaxY;
};