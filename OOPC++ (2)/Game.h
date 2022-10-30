#pragma once
#include <windows.h>
#include <vector>
#include <string>

//TODO гибкое количество клеток с разными правилами и их конвертация в соответствующие иконки

#define EMPTY_CELL ' '
#define OCCUPIED_CELL '#'

constexpr SHORT FILED_BASE_SIZE = 15;

enum Cell_T {
	empty, cell
};

typedef std::vector<std::wstring> FrameData;
typedef std::vector<std::vector<Cell_T>> FieldData;

class Field 
{
public:
	Field();
	Field(const SHORT&, const SHORT&);
	~Field() = default;

	Cell_T& at(SHORT, SHORT);

	bool placeCell(const SHORT&, const SHORT&);
	bool clearCell(const SHORT& x, const SHORT& y);

	SHORT width;
	SHORT height;
	FieldData data;
};

class Frame
{
public:
	Frame(const Field&);
	Frame(const SHORT&, const SHORT&, const FrameData&);
	Frame(const SHORT&, const SHORT&);
	~Frame() = default;

	SHORT width;
	SHORT height;
	FrameData data;
};
