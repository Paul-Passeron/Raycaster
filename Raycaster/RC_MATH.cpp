//RC_MATH.cpp

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
	y = y * (threehalfs - (x2 * y * y)); //Second iteration : necessary ?
	return y;
}

