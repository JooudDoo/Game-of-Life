#pragma once
#include "utility.h"

#include <fstream>

constexpr auto standartLogName = "ParserLog";
constexpr auto lifeVerBlank = "#Life ";
constexpr auto lifeNameBlank = "#N ";
constexpr auto lifeSizeBlank = "#S ";
constexpr auto lifeRuleBlank = "#R ";

class FileParser {
public:
	FileParser();
	~FileParser();

	GameData parseUniverseFF(const std::string&); //parse universe from file [poleznoe]

private:

	std::ofstream logFile;
	bool checkBlank(const std::string&, const std::string&, std::string&);
	std::pair<SHORT, SHORT> checkCell(const std::string&);

};