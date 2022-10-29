/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_MATH.hpp
================================================================
*/

#pragma once

float fast_inverse_square_root(float x) {
	/*
	================================================================
	fast_inverse_square_root(float x) -> float:
	returns the inverse square root of the float number x using
	Quake III's fast inverse square root algorithm.
	================================================================
	*/
	long i;
	float x2, y;
	const float threehalfs = 1.5f;
	x2 = x * 0.5f;
	y = x;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2 * y * y));
	//y = y * (threehalfs - (x2 * y * y)); //Second iteration : necessary ?
	return y;
}

int indexBiggestElem(float* Array, int n) {
	/*
	================================================================
	indexBiggestElem(float* Array) -> int:
	returns the index of the element with biggest value in the
	array. We use floats as it is more convenient for the overall
	code.
	================================================================
	*/
	int maxi = 0;
	for (int i = 1; i < n; i++) {
		
		if (Array[maxi] <= Array[i]) {
			maxi = i;
		}
	}
	return maxi;
}

float min(float a, float b) {
	if (a < b) {
		return a;
	}
	return b;
}

float max(float a, float b) {
	return a + b - min(a, b);
}

float min(float a, float b, float c, float d) {
	if (a > b && a > c && a > d) { return a; }
	if (b > a && b > c && b > d) { return b; }
	if (c > a && c > b && c > d) { return c; }
	return d;
}