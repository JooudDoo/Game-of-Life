#pragma once
#include "utility.h"

class GameLogic {
public:
	GameLogic();
	GameLogic(const SHORT&, const SHORT&);
	GameLogic(const SHORT&, const SHORT&, const GameRule&);
	~GameLogic() = default;

	void simulate();
	void simulateTicks(const int&);

	bool isEmptyCell(const SHORT& x, const SHORT& y);
	bool switchCell(const SHORT& x, const SHORT& y);
	bool placeCell(const SHORT& x, const SHORT& y);
	bool clearCell(const SHORT& x, const SHORT& y);
	bool setBlankField(const Field&);
	void cleanField();
	void clearField();

	void setRule(const GameRule&);
	GameRule getRule() const;

	Field& getField();

private:
	Field gameField;
	Field blankField;
	GameRule rule;
	SHORT width;
	SHORT height;

	SHORT checkAliveSquare(const SHORT&, const SHORT&);
};