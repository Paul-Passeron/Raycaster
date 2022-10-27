/*
================================================================
RayCaster Engine
Paul Passeron 2022
RC_GEOM.hpp
================================================================
*/
#pragma once

#include "RC_MATH.hpp"

namespace geom {
	typedef enum lineLineIntersectionArgs {
		FALLS_WITHIN_FIRST_LINE_SEGMENT,
		FALLS_WITHIN_SECOND_LINE_SEGMENT,
	};

	typedef struct point {
		/*
		================================================================
		Strucuture representing a point in euclidian space with
		cartesian coordinates x and y and a boolean used as a flag
		telling wether the point exists or not. Default ocnstructor
		sets exists to false as it helps produce more readable code as
		a point exists is checked only when doing wall rendering, not
		player movement etc...
		================================================================
		*/
		float x;
		float y;
		bool exists;
		//Constructor used most of the time
		point(float x_, float y_, bool exists_ = false) {
			x = x_; y = y_; exists = exists_;
		}
		//Default constructor, here exists will be false.
		point() {
			point(0, 0);
		}
	};

	typedef struct line {
		/*
		================================================================
		Strucuture representing a line in euclidian space using two
		points p1 and p2 (point struct above). default constructor
		creates a line that isn't really a line as it is defined by
		the point (0, 0) for p1 and p2.
		================================================================
		*/
		point p1;
		point p2;
		//Constructor used most of the time
		line(point pa, point pb) {
			p1 = pa;
			p2 = pb;
		}
		//Default constructor.
		line() {
			line(point(), point());
		}
	};

	 typedef struct player {
		 /*
		================================================================
		Structure representing the player, it has a position (pos) and
		an angle (angle) which indicates which way the player is
		looking.
		================================================================
		*/
		point pos;
		float angle;
		//Constructor used most of the time. Noting that we don't really
		//care about the value of pos.exists as it's never checked.
		player(point pos_, float angle_) {
			pos = pos_; angle = angle_;
		}
		//Default constructor, sets 0 as the angle and (0, 0) as the pos.
		player() {
			player(point(), 0);
		}
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
		vice-versa. It uses the formulas found on Wikipedia.
		================================================================
		*/
		//Local variable declarations, only for clarity.
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

		//if den = 0 then we can't calculate the ratios so the point doesn't exists.
		if (den == 0) {
			return point();
		}
		//We calculate u and t (We maybe could optimize this part).
		float t = t_num / den;
		float u = u_num / den;

		point p;

		switch (arg) {
			//Do we want the point to be contained in the 1st line segment ?
			//If not it doesn't exist
			case FALLS_WITHIN_FIRST_LINE_SEGMENT:
				p.exists = 0 <= t && t <= 1;
				break;
			//Same but for the second line segment.
			case FALLS_WITHIN_SECOND_LINE_SEGMENT:
				p.exists = 0 <= u && u <= 1 && t_num * u_num >= 0;
				break;
		}
		//Calculating the point's coordinates.
		p.x = x1 + t * (x2 - x1);
		p.y = y1 + t * (y2 - y1);

		return p;
	}


	float distance_point_line(line l, point p) {
		/*
		================================================================
		Returns the minimum distance between a point and a line. It is
		used to correct the kind of "fisheye" effect we get if we take
		the distance from the player to the wall rather than from a
		projection plane to the wall. It uses a formula found on
		Wikipedia alongside Quake III's fast inverse square root
		algorithm.
		================================================================
		*/
		//naming local constants for convenience reasons.
		float x0 = p.x;
		float y0 = p.y;
		float x1 = l.p1.x;
		float y1 = l.p1.y;
		float x2 = l.p2.x;
		float y2 = l.p2.y;
		float den = inverse_distance(l.p1, l.p2);
		float num = abs((x2 - x1) * (y1 - y0) - (x1 - x0) * (y2 - y1));
		//Here it's * den because den is actually 1 / the actual denominator
		//because we use the fast inverse square root algorithm to speed
		//everything up.
		return num * den;
	}
}

