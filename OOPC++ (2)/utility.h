#pragma once
#include <windows.h>
#include <vector>
#include <string>

#define NO_WCHAR__

#ifdef WCHAR__
#define STRING std::wstring
#define COUT std::wcout
#define CHAR_T wchar_t
#define CP CP_UTF8
#else
#define STRING std::string
#define COUT std::cout
#define CHAR_T char
#define CP 866
#endif

enum ConsoleInteractiveCode {
	none,
	quit,
	pause,
	mouseClick,
	reset,
	saveFieldToReset,
	mouseCtrlMove,
	consoleMode,
};

enum ConsoleWriteCode {
	NAC, //NotACommand
	offConMode, //Switch from console mode
	tickSkip, // Simulate some ticks
	cleanField, // clean field
	help, //Show help annotation to all commands
};

std::string consoleInterCodeToString(const ConsoleInteractiveCode&);

typedef struct PlAction_S{

	ConsoleInteractiveCode code;
	COORD mouseClick;
	BOOL ctrlState;

}PlAction;

constexpr auto EMPTY_CELL = ' ';//'\xb0';
constexpr auto OCCUPIED_CELL = '\xdb';
constexpr auto EMPTY_CELL_ATR = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
constexpr auto OCCUPIED_CELL_ATR = 0;

constexpr CHAR_T BORDERCORNER = '\xce';
constexpr CHAR_T BORDERVERTICAL = '\xba';
constexpr CHAR_T BORDERHORIZONTAL = '\xcd';

constexpr SHORT WIDTHDEFAULT = 25;
constexpr SHORT HEIGHTDEFAULT = 25;

constexpr SHORT FILED_BASE_SIZE = 15;

enum Cell_T {
	empty, cell
};

typedef std::vector<STRING> FrameData;
typedef std::vector<std::vector<DWORD>> FrameAtrData;
typedef std::vector<std::vector<Cell_T>> FieldData;

typedef struct CommandAlias_S {
	std::vector<std::string> alias;
	ConsoleWriteCode code;
	std::string helpPopUp;
}CommandAlias;

static std::vector<CommandAlias> aliasesForWriteCodes({
	{{"exit", "-e", "quit", "off"}, offConMode, "To exit console mode: "},
	{{"clean", "cleanField", "-cF", "-cf"}, cleanField, "To clear the field of the universe : "},
	{{"tick", "-t", "tickSkip", "ticks", "-tick"}, tickSkip, "To simulate N frames in offline mode: "},
	{{"help", "-h"}, help, "Show this message: "}
	});

inline bool isNumber(std::string line)
{
	char* p = NULL;
	strtol(line.c_str(), &p, 10);
	return *p == NULL;
}

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

template<typename DataType, typename LineType, typename SymType>
class FrameBlank
{
public:
	FrameBlank();
	FrameBlank(const Field&);
	FrameBlank(const SHORT&, const SHORT&, const DataType&);
	FrameBlank(const SHORT&, const SHORT&);
	SymType at(SHORT, SHORT) const;
	LineType line(SHORT) const;
	~FrameBlank() = default;

	SHORT width;
	SHORT height;
	DataType data;
};

class Frame
{
public:
	Frame();
	Frame(const Field&);
	Frame(const SHORT&, const SHORT&, const FrameData&);
	Frame(const SHORT&, const SHORT&);
	CHAR_T at(SHORT, SHORT) const;
	STRING line(SHORT) const;
	~Frame() = default;

	SHORT width;
	SHORT height;
	FrameData data;
};

//#include "Frame.h"
//typedef FrameBlank<FrameData, STRING, CHAR_T> Frame;
//typedef FrameBlank<FrameAtrData, std::vector<DWORD>, DWORD> FrameAtr;

typedef struct GameRule_S {
	std::vector<SHORT> birth;
	std::vector<SHORT> stay;
}GameRule;

typedef struct GameData_S {

	std::string name;
	Field gameField;
	GameRule rule;

}GameData;

GameRule ruleFString(const std::string&);
std::pair<SHORT, SHORT> sizeFString(const std::string&);

