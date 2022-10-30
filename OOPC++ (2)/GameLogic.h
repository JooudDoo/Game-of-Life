#pragma once
#include "Game.h"

typedef struct GameRule_S {
	std::vector<SHORT> birth;
	std::vector<SHORT> stay;
}GameRule;

static GameRule DEFAULT_RULE = { {3}, {2, 3} };

class GameLogic {
public:
	GameLogic(const SHORT&, const SHORT&);
	GameLogic(const SHORT&, const SHORT&, const GameRule&);
	~GameLogic() = default;

	void simulate();

	bool placeCell(const SHORT&, const SHORT&);
	bool clearCell(const SHORT& x, const SHORT& y);

	Field& getField();

private:
	Field gameField;
	GameRule rule;
	SHORT width;
	SHORT height;

	SHORT checkAliveSquare(const SHORT&, const SHORT&);
};