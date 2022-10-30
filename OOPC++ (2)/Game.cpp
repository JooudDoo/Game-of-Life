#include "Game.h"

Frame::Frame() : width(0), height(0), data(NULL) {

}

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

CHAR_T& Frame::at(SHORT y, SHORT x) {
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

Field::Field() : width(FILED_BASE_SIZE), height(FILED_BASE_SIZE) {};

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
