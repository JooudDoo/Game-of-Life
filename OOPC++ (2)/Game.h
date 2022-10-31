#pragma once
#include "utility.h"
#include "GameRenderer.h"
#include "GameLogic.h"

#include <sys/timeb.h>
typedef _timeb sys_time_t;

//TODO гибкое количество клеток с разными правилами и их конвертация в соответствующие иконки
//TODO при рендере изменять только то что изменяется, а не всё поле ✓
//TODO UTF-8 support ✓ (Частично)
//TODO TPS meter ✓
//TODO переделать основный цикл игры ✓ (не так как задумывалось)
//TODO rework state position for mouse handler ✓ (не так как задумывалось)
//TODO make border for mouse click


class LifeGame {
public:
	LifeGame();
	LifeGame(const SHORT& cWidth, const SHORT& cHeight);

	bool placeCell(const SHORT& x, const SHORT& y);
	bool clearCell(const SHORT& x, const SHORT& y);
	void runGame(const std::string& universeName, const SHORT& targetTPS);
	void renderFrame(const bool&, const SHORT&, const ConsoleCodes&);

private:

	void pausedGame();

	sys_time_t T_st, T_end;
	SHORT targetTPS;
	GameLogic logic;
	GameRenderer renderer;
	SHORT canvasWidth;
	SHORT canvasHeight;
};

