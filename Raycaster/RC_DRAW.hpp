#pragma once
int get_Index(int i, int j, int width) {
	/*
	================================================================
	get_index(int i, int j, int width) -> int:
	converts 2d array index (i, j) of size (width, height) to 1d
	array of size width * height.
	================================================================
	*/
	return i + j * width;
}
