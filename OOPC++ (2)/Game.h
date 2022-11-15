#pragma once
#include "utility.h"
#include "Console.h"
#include "GameLogic.h"
#include "Parser.h"

#include <sys/timeb.h>
typedef _timeb sys_time_t;

enum GameState {
	gameNone, gameWork, gamePause, gameConsole
};

//TODO гибкое количество клеток с разными правилами и их конвертация в соответствующие иконки
//TODO при рендере изменять только то что изменяется, а не всё поле ✓
//TODO UTF-8 support ✓ (Частично)
//TODO TPS meter ✓
//TODO переделать основный цикл игры ✓ (не так как задумывалось)
//TODO rework state position for mouse handler ✓ (не так как задумывалось)
//TODO make border for mouse click
//TODO rework frame render (Performance boost) ✓

class LifeGame {
	friend LifeGame createGameFFile(const std::string&);
public:
	LifeGame();
	LifeGame(const SHORT& cWidth, const SHORT& cHeight);

	bool placeCell(const SHORT& x, const SHORT& y);
	bool clearCell(const SHORT& x, const SHORT& y);
	bool setFieldBlank(const Field&);
	
	void setUniverseName(const std::string&);
	void setTargetTPS(const SHORT&);
	void setGameRule(const GameRule&);

	void runGame();
	void renderFrame(const bool&, const ConsoleInteractiveCode&);

private:

	std::string universeName;

	void pausedGame();
	void consoledGame();
	void resetField();

	sys_time_t T_st, T_end;
	SHORT targetTPS;
	DWORD TargetDelay;
	GameLogic logic;
	Console con;
	SHORT canvasWidth;
	SHORT canvasHeight;

	GameState gameStatus;
};

LifeGame createGameFFile(const std::string&);