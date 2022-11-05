#pragma once
#include "utility.h"

class GameLogic {
public:
	GameLogic();
	GameLogic(const SHORT&, const SHORT&);
	GameLogic(const SHORT&, const SHORT&, const GameRule&);
	~GameLogic() = default;

	void simulate();

	bool switchCell(const SHORT& x, const SHORT& y);
	bool placeCell(const SHORT& x, const SHORT& y);
	bool clearCell(const SHORT& x, const SHORT& y);
	bool setBlankField(const Field&);
	void clearField();

	void setRule(const GameRule&);

	Field& getField();

private:
	Field gameField;
	Field blankField;
	GameRule rule;
	SHORT width;
	SHORT height;

	SHORT checkAliveSquare(const SHORT&, const SHORT&);
};