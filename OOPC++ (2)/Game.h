#pragma once
#include "utility.h"

#include "GameRenderer.h"
#include "GameLogic.h"



//TODO гибкое количество клеток с разными правилами и их конвертация в соответствующие иконки
//TODO при рендере изменять только то что изменяется, а не всё поле ✓
//TODO UTF-8 support ✓ (Частично)
//TODO переделать основный цикл игры
//TODO rework state position for mouse handler
//TODO make border for mouse click


class LifeGame {
public:
	LifeGame();
	LifeGame(const SHORT& cWidth, const SHORT& cHeight);

	bool placeCell(const SHORT& x, const SHORT& y);
	bool clearCell(const SHORT& x, const SHORT& y);
	void runGame(const SHORT& FPS);
	void renderFrame(const bool&);

private:
	GameLogic logic;
	GameRenderer renderer;
	SHORT canvasWidth;
	SHORT canvasHeight;
};

