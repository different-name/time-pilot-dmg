#include "calc.h"

#include "main.h"

// Velocity lookup table, precalculated to save resources
// Calculated using sin(n * 22.5 deg) * 255 where n = {0, 1, 2, 3, 4, 3, 2, 1};
const uint8_t VELOCITIES[8] = {0, 98, 180, 236, 255, 236, 180, 98};

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

// Increments / decrements rotation by 1 toward target rotation
int8_t step_to_rotation(int8_t r1, int8_t r2) {
	if (r1 == r2) return r1; // Do not step if rotations match

	// Calculate direction to step
	int8_t direction = ((r2 - r1 + MAX_ROTATION) % MAX_ROTATION <= HALF_ROTATION) ? 1 : -1;

	return (r1 + direction + MAX_ROTATION) % MAX_ROTATION; // Step toward target rotation
}

// Calculate the velocity components using the distance travelling and the orientation
Vector2D velocity_from_rotation_and_distance(int8_t rotation, uint16_t distance) {
	Vector2D velocity;

	velocity.x = VELOCITIES[rotation % HALF_ROTATION]; // Use velocity lookup table
	velocity.x = (velocity.x * distance) / 255;		   // How many subpixels to travel
	velocity.x *= rotation > HALF_ROTATION ? -1 : 1;   // Invert direction if needed

	velocity.y = VELOCITIES[(rotation + QUARTER_ROTATION) % HALF_ROTATION];
	velocity.y = (velocity.y * distance) / 255;
	velocity.y *= rotation > QUARTER_ROTATION && rotation < (QUARTER_ROTATION * 3) ? 1 : -1;

	return velocity;
}