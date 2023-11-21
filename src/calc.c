#include "calc.h"
#include "main.h"

// Calculates the angle between two points as a 8 bit unsigned integer (0 to 15)
uint8_t angle_between_pos(Pos p1, Pos p2) {
	if (p1.x == p2.x) { // x coordinate is same
		return (p1.y < p2.y) * 8; // Point up (0) or down (8)
	}
	if (p1.y == p2.y) { // y coordinate is same
		return (p1.x > p2.x) * 8 + 4; // Point right (4) or left (12)
	}

	uint8_t left = p2.x < p1.x; // Target x is less than start x (target left of start)
	uint8_t below = p2.y > p1.y; // Target y is greater than start y (target below start)

	uint8_t angle = ((left << 1) | (left ^ below)) * 4; // Calculating quadrant as a number from 0 to 3, then multiplying by 4

	uint8_t dx = (p1.x > p2.x) ? p1.x - p2.x : p2.x - p1.x; // x distance between p1 and p2
	uint8_t dy = (p1.y > p2.y) ? p1.y - p2.y : p2.y - p1.y; // y distance between p1 and p2

	uint16_t slope = (left ^ below) ? (dy * 100) / dx : (dx * 100) / dy; // Calculate the slope between points, flip x and y axis for top left or bottom right quadrants

	angle += (slope >= 20) + (slope >= 67) + (slope >= 150) + (slope >= 503); // Increment the angle if the slope is greater than certain values (instead of performing arctan approximation, check against pre-calculated values)

	return angle;
}