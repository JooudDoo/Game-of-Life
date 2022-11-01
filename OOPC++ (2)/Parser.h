#include "utility.h"

constexpr auto lifeVerBlank = "#Life ";

class FileParser {
public:
	FileParser();
	~FileParser() = default;

	GameData parseUniverseFF(const std::string&); //parse universe from file [poleznoe]

private:

	bool checkLifeVer(const std::string&, std::string&);
};