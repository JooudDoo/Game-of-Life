template<typename DataType, typename LineType, typename SymType>
FrameBlank<typename DataType, typename LineType, typename SymType>::FrameBlank() : width(0), height(0), data(NULL) {}

template<typename DataType, typename LineType, typename SymType>
FrameBlank<typename DataType, typename LineType, typename SymType>::FrameBlank(const SHORT& iWidth, const SHORT& iHeight, const DataType& iData) : width(iWidth), height(iHeight), data(iData) {}

template<>
FrameBlank<FrameData, STRING, CHAR_T>::FrameBlank(const Field& gf) : width(gf.width), height(gf.height) {
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

template<>
FrameBlank<FrameAtrData, std::vector<DWORD>, DWORD>::FrameBlank(const Field& gf) : width(gf.width), height(gf.height) {
	data = FrameAtrData();
	data.resize(height);
	for (SHORT i = 0; i < height; i++) {
		for (auto el : gf.data[i]) {
			if (el != 0) {
				data[i].push_back(OCCUPIED_CELL_ATR);
			}
			else {
				data[i].push_back(EMPTY_CELL_ATR);
			}
		}
	}
}

template<>
FrameBlank<FrameData, STRING, CHAR_T>::FrameBlank(const SHORT& iWidth, const SHORT& iHeight) : width(iWidth), height(iHeight) {
	data = FrameData();
	data.resize(height, STRING(width, EMPTY_CELL));
}

template<>
FrameBlank<FrameAtrData, std::vector<DWORD>, DWORD>::FrameBlank(const SHORT& iWidth, const SHORT& iHeight) : width(iWidth), height(iHeight) {
	data = FrameAtrData();
	data.resize(height, std::vector<DWORD>(width, EMPTY_CELL_ATR));
}


template<typename DataType, typename LineType, typename SymType>
SymType FrameBlank<typename DataType, typename LineType, typename SymType>::at(SHORT y, SHORT x) const {
	x += width;
	y += height;
	y %= height;
	x %= width;
	return data[y][x];
}

template<typename DataType, typename LineType, typename SymType>
LineType FrameBlank<typename DataType, typename LineType, typename SymType>::line(SHORT y) const {
	y += height;
	y %= height;
	return data[y];
}