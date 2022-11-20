#include "utility.h"

#include <regex>

static GameRule DEFAULTRULE = { {3}, {2, 3} };

std::string consoleInterCodeToString(const ConsoleInteractiveCode& c) {
	switch (c) {
	default:
	case(none):
		return "simulation";
	case(quit):
		return "disabled";
	case(pause):
		return "paused";
	case(mouseClick):
		return "mouse mode";
	case(reset):
		return "reset field";
	case(consoleMode):
		return "console";
	}
}

Frame::Frame() : width(0), height(0), data(NULL) {}

Frame::Frame(const SHORT& iWidth, const SHORT& iHeight, const FrameData& iData) : width(iWidth), height(iHeight), data(iData) {}

Frame::Frame(const Field& gf) : width(gf.width), height(gf.height) {
	data = FrameData();
	data.resize(height);
	for (SHORT i = 0; i < height; i++) {
		for (auto el : gf.data[i]) {
			if (el != 0) {
				data[i].push_back(OCCUPIED_CELL);
			}
			else {
				data[i].push_back(EMPTY_CELL);
			}
		}
	}
}

Frame::Frame(const SHORT& iWidth, const SHORT& iHeight) : width(iWidth), height(iHeight) {
	data = FrameData();
	data.resize(height, STRING(width, EMPTY_CELL));
}

CHAR_T Frame::at(SHORT y, SHORT x) const {
	x += width;
	y += height;
	y %= height;
	x %= width;
	return data[y][x];
}

STRING Frame::line(SHORT y) const {
	y += height;
	y %= height;
	return data[y];
}

Field::Field() : width(FIELD_BASE_SIZE), height(FIELD_BASE_SIZE) {
	data = FieldData();
	data.resize(height, std::vector<Cell_T>(width));
};

Field::Field(const SHORT& iWidth, const SHORT& iHeight) : width(iWidth), height(iHeight) {
	data = FieldData();
	data.resize(height, std::vector<Cell_T>(width));
}

bool Field::clearCell(const SHORT& y, const SHORT& x) {
	if (this->at(y, x) == empty) {
		return false;
	}
	this->at(y, x) = empty;
	return true;
}

bool Field::placeCell(const SHORT& y, const SHORT& x) {
	if (this->at(y, x) != empty) {
		return false;
	}
	this->at(y, x) = cell;
	return true;
}

Cell_T& Field::at(SHORT y, SHORT x) {

	if (y >= height) {
		y %= height;
	}
	if (x >= width) {
		x %= width;
	}
	if (x < 0) {
		x += width;
	}
	if (y < 0) {
		y += height;
	}
	return data[y][x];
}

// B2/S23
GameRule ruleFString(const std::string& rule) {
	GameRule rl;
	std::regex birthPattern("B([0-9]*)/");
	std::regex stayPattern("S([0-9]*)");

	std::smatch birth;
	std::smatch stay;
	std::regex_search(rule, birth, birthPattern);
	std::regex_search(rule, stay, stayPattern);

	auto seq = birth.begin() + 1;
	if (seq == birth.end())
		return DEFAULTRULE;
	std::string parse = *seq;
	for (auto sym : parse) {
		rl.birth.push_back(sym - '0');
	}
	seq = stay.begin() + 1;
	if (seq == stay.end())
		return DEFAULTRULE;
	parse = *seq;
	for (auto sym : parse) {
		rl.stay.push_back(sym - '0');
	}

	return rl;
}

std::pair<SHORT, SHORT> sizeFString(const std::string& str) {
	int m1, m2;
	std::regex birthPattern("([0-9]*).?");
	std::regex stayPattern("([0-9]*)");

	std::smatch birth;
	std::smatch stay;
	std::regex_search(str, birth, birthPattern);
	std::string a = birth.suffix().str();
	std::regex_search(a, stay, stayPattern);

	auto seq = birth.begin() + 1;
	if (seq == birth.end())
		return {15, 15};
	std::string parse = *seq;
	sscanf_s(parse.c_str(), "%d", &m1);
	seq = stay.begin() + 1;
	if (seq == stay.end())
		return  { 15, 15 };
	parse = *seq;
	sscanf_s(parse.c_str(), "%d", &m2);
	

	return {m1, m2};
}