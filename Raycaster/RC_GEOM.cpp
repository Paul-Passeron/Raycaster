//RC_GEOM.cpp

#include "RC_MATH.h"

enum lineLineIntersectionArgs{
	FALLS_WITHIN_FIRST_LINE_SEGMENT,
	FALLS_WITHIN_SECOND_LINE_SEGMENT,
};

struct point {
/*
================================================================
Strucuture representing a point in euclidian space with
cartesian coordinates x and y and a boolean used as a flag
telling wether the point exists or not.
================================================================
*/
	float x;
	float y;
	bool exists;
};

struct line {
/*
================================================================
Strucuture representing a line in euclidian space using two
points p1 and p2 (point struct above).
================================================================
*/
	point p1;
	point p2;
};



float distance_squared(point p1, point p2) {
/*
================================================================
distance_squared(point p1, point p2) -> float:
takes to points p1 and p2 as parameters and returns the squared
euclidian distance between them.
================================================================
*/
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}


float inverse_distance(point p1, point p2) {
/*
================================================================
inverse_distance(point p1, point p2) -> float:
takes to points p1 and p2 as parameters and returns an
approximation of the inverse square root of the euclidian
distance between them (uses Quake III's fast inverse square
root algorithm.
================================================================
*/
	return fast_inverse_square_root(distance_squared(p1, p2));
}

point line_line_intersection(line l1, line l2, lineLineIntersectionArgs arg) {
/*
================================================================
line_line_intersection(line l1, line l2, args) -> point:
returns the point of intersection between line l1 and l2. If
arg is FALL_WITHIN_FIRST_LINE_SEGMENT the point's "exist" field
is true iff the point falls within the first line segment and
vice-versa.
================================================================
*/
	//Local variable declarations, its single-use is readability.
	float x1 = l1.p1.x;
	float y1 = l1.p1.y;
	float x2 = l1.p2.x;
	float y2 = l1.p2.y;
	float x3 = l2.p1.x;
	float y3 = l2.p1.y;
	float x4 = l2.p2.x;
	float y4 = l2.p2.y;
	float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	float t_num = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
	float u_num = (x1 - x3) * (y1 - y2) - (y1 - y3) * (x1 - x2);
	point p;
	//if den = 0 then we can't calculate the ratios so the point doesn't exists.
	if (den == 0) {
		p.exists = false;
		return p;
	}
	//We calculate u and t (We maybe could optimize this part).
	float t = t_num / den;
	float u = u_num / den;
	
	switch (arg){
		case FALLS_WITHIN_FIRST_LINE_SEGMENT:
			p.exists = 0 <= t && t <= 1;
			break;
		case FALLS_WITHIN_SECOND_LINE_SEGMENT:
			p.exists = 0 <= u && u <= 1;
			break;
		default:
			p.exists = false;
	}
	p.x = x1 + t * (x2 - x1);
	p.y = y1 + t * (y2 - y1);
	return p;
}