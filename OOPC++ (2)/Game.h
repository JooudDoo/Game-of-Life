#pragma once
#include "utility.h"

#include "GameRenderer.h"
#include "GameLogic.h"



//TODO гибкое количество клеток с разными правилами и их конвертация в соответствующие иконки
//TODO при рендере изменять только то что изменяется, а не всё поле ✓
//TODO UTF-8 support ✓ (Частично)


class LifeGame {
public:
	LifeGame();

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

