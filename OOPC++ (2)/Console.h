#ifndef ConsoleH___
#define ConsoleH___
#include "utility.h"
#include <sys/timeb.h>
typedef _timeb sys_time_t;

constexpr COORD topLeft = { 0, 0 };

constexpr SHORT consoleWritePadding = 2;
constexpr auto consoleWriteUserInputPrefix = ">> ";

constexpr auto consoleWriteHelpPrefix = "??> ";
constexpr auto consoleWriteAnnotationPrefix = "<<< ";
constexpr auto consoleWriteWarningPrefix = "!! ";

constexpr auto consoleWriteParameterHelpPopUpSym = "<param>";
constexpr auto nullConsoleParametr = "-1";


typedef struct ConsolePreferences_s {
	HANDLE cInput;
	HANDLE cOut;
	COORD menuPos;
	SHORT consoleMaxX;
	SHORT consoleMaxY;
	SHORT canvasHeight;
	SHORT canvasWidth;
	COORD canvasStartPos;
	SHORT canvasSquare;

}ConsolePreferences;


class GameRenderer
{
	friend class Console;
public:
	GameRenderer() = default;
	GameRenderer(const ConsolePreferences&);
	~GameRenderer() = default;

	void prepConsole();

	void renderFrame(const Frame&);
	void renderFrame(const Field&);
	void renderGUI(const SHORT&, const ConsoleInteractiveCode&);

	void writeLineWithPrefix(const std::string& line, const char* prefix);

	void setUniverseName(const std::string&);
	void setTargetFPS(const SHORT&);

	void setFocusMousePosition(const COORD&);
	BOOL isIntMode;
private:
	void clearConsole();
	void clearWriteConsole(const SHORT&);
	void initCanvas();
	void renderBorder();

	void drawSymbol(const SHORT& x, const SHORT& y, CHAR_T sym);
	bool setCursorPos(const COORD&);

	void renderName();
	void renderFPS(const SHORT& currentFPS);
	void renderState(const ConsoleInteractiveCode&);
	void renderInstruction();

	int64_t frameDiff(const Frame&, const Frame&);
	void renderFrameByLine(const Frame&);
	void renderFrameByPix(const Frame&);

	ConsolePreferences cnPref;
	COORD focusMousePosition;
	Frame prevFrame;
	BOOL isNameRendered;
	BOOL isInstRendered;
	std::string universeName;
	SHORT targetFPS;
	ConsoleInteractiveCode prevState;
	SHORT prevFPS;
};

class GameInput {
public:
	GameInput() = default;
	GameInput(const ConsolePreferences&);
	~GameInput() = default;
	std::pair<ConsoleWriteCode, std::string> proccesInput();
	std::vector<PlAction> checkPlayer(const bool& useMouse);
private:
	PlAction keyboardHandler(const KEY_EVENT_RECORD&);
	PlAction mouseHandler(const MOUSE_EVENT_RECORD&, const bool&);
	ConsolePreferences cnPref;
};

class Console {
public:
	Console();
	Console(const SHORT&, const SHORT&);
	std::vector<PlAction> getPlayerActions(const bool&);
	void renderConsoleFrame(const Frame&, const ConsoleInteractiveCode&);
	void consoleWriteProcessed();
	void writeLineWithPrefix(const std::string& line, const char* prefix);

	void clearWriteConsole();
	void switchToConsoleMode();
	void switchFromConsoleMode();

	GameRenderer gRen;
	GameInput gInp;

private:
	int frameCounter;

	void InitConsole();
	SHORT calculateFPS();
	SHORT lastConsoleLine;
	SHORT currentFPS;

	sys_time_t T_st;
	sys_time_t T_end;

	ConsolePreferences cnPref;
	bool isSetted;
};

#endif