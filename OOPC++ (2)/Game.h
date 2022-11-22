#pragma once
#include "utility.h"
#include "Console.h"
#include "GameLogic.h"
#include "FileWorkers.h"

#include <sys/timeb.h>
typedef _timeb sys_time_t;

enum GameState {
	gameNone, gameWork, gamePause, gameConsole
};

constexpr SHORT DEFAULT_INITIAL_FPS = 60;
constexpr SHORT DEFAULT_PAUSED_INITIAL_FPS = 244;

//TODO гибкое количество клеток с разными правилами и их конвертация в соответствующие иконки
//TODO при рендере изменять только то что изменяется, а не всё поле ✓
//TODO UTF-8 support ✓ (Частично)
//TODO TPS meter ✓
//TODO переделать основный цикл игры ✓ (не так как задумывалось)
//TODO rework state position for mouse handler ✓ (не так как задумывалось)
//TODO make border for mouse click
//TODO rework frame render (Performance boost) ✓

class LifeGame {
	friend LifeGame createGameFFile(const std::string& filePath, LifeGame* prevGame);
public:
	LifeGame();
	LifeGame(const SHORT& cWidth, const SHORT& cHeight);
	~LifeGame() = default;

	bool placeCell(const SHORT& x, const SHORT& y);
	bool clearCell(const SHORT& x, const SHORT& y);
	bool setFieldBlank(const Field&);
	
	void setUniverseName(const std::string&);
	void setTargetFPS(const SHORT&);
	void setGameRule(const GameRule&);

	void runGame();
	void renderFrame(const bool&, const ConsoleInteractiveCode&);

private:

	std::string universeName;

	void pausedGame();
	void consoledGame();
	void setBlankField();
	void clearField();

	SHORT targetFPS;
	DWORD targetDelay;
	SHORT pauseTargetFPS;
	DWORD pauseTargetDelay;
	GameLogic logic;
	Console con;
	SHORT canvasWidth;
	SHORT canvasHeight;

	GameState gameStatus;
};

LifeGame createGameFFile(const std::string& filePath, LifeGame* prevGame = nullptr);