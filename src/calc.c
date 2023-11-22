#include "calc.h"

#include "main.h"

// Calculates the direction between two points
int8_t direction_to_point(Point p1, Point p2) {
	if (p1.x == p2.x) { // x coordinate is same
		// Point up or down
		return (p1.y < p2.y) * HALF_ROTATION;
	}
	if (p1.y == p2.y) { // y coordinate is same
		// Point right or left
		return (p1.x > p2.x) * HALF_ROTATION + QUARTER_ROTATION;
	}

	uint8_t dx = (p1.x > p2.x) ? p1.x - p2.x : p2.x - p1.x; // x distance between p1 and p2
	uint8_t dy = (p1.y > p2.y) ? p1.y - p2.y : p2.y - p1.y; // y distance between p1 and p2

	uint8_t left = p2.x < p1.x;	 // Target x is less than start x (target left of start)
	uint8_t below = p2.y > p1.y; // Target y is greater than start y (target below start)

	// Calculating direction first as the quadrant, which will be incremented by the slope
	int8_t direction = ((left << 1) | (left ^ below)) * QUARTER_ROTATION;

	// Calculate the slope between points, flip x and y axis for top left or bottom right quadrants
	uint16_t slope = (left ^ below) ? (dy * 100) / dx : (dx * 100) / dy;

	// Increment the angle if the slope is greater than certain values (instead of performing arctan
	// approximation, check against pre-calculated values)
	direction += (slope >= 20) + (slope >= 67) + (slope >= 150) + (slope >= 503);

	return direction;
}