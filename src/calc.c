#include "calc.h"

#include "main.h"

// Velocity lookup table, precalculated to save resources
// Calculated using sin(n * 22.5 deg) * 255 where n = {0, 1, 2, 3, 4, 3, 2, 1};
const uint8_t VELOCITIES[8] = {0, 98, 180, 236, 255, 236, 180, 98};

// Calculates the direction between two points
uint8_t direction_to_point(UVector8* p1, UVector8* p2) {
	if (p1->x == p2->x) { // x coordinate is same
		// Point up or down
		return (p1->y < p2->y) * 8;
	}
	if (p1->y == p2->y) { // y coordinate is same
		// Point right or left
		return (p1->x > p2->x) * 8 + 4;
	}

	uint8_t dx = (p1->x > p2->x) ? p1->x - p2->x : p2->x - p1->x; // x distance between p1 and p2
	uint8_t dy = (p1->y > p2->y) ? p1->y - p2->y : p2->y - p1->y; // y distance between p1 and p2

	uint8_t left = p2->x < p1->x;  // Target x is less than start x (target left of start)
	uint8_t below = p2->y > p1->y; // Target y is greater than start y (target below start)

	// Calculating direction first as the quadrant, which will be incremented by the slope
	int8_t direction = ((left << 1) | (left ^ below)) * 4;

	// Calculate the slope between points, flip x and y axis for top left or bottom right quadrants
	uint16_t slope = (left ^ below) ? (dy * 100) / dx : (dx * 100) / dy;

	// Increment the angle if the slope is greater than certain values (instead of performing arctan
	// approximation, check against pre-calculated values)
	direction += (slope >= 20) + (slope >= 67) + (slope >= 150) + (slope >= 503);

	return direction & 15;
}

// Increments / decrements rotation by 1 toward target rotation
uint8_t step_to_rotation(int8_t r1, int8_t r2) {
	if (r1 == r2) return r1; // Do not step if rotations match

	// Calculate direction to step
	int8_t direction = ((r2 - r1 + 16) % 16 <= 8) ? 1 : -1;

	return (r1 + direction + 16) & 15; // Step toward target rotation
}

void movement_from_velocity(GameObject* gameObject, Vector8* gameObject_movement) {
	UVector8 gameObject_velocity = {VELOCITIES[gameObject->rotation & 7],
									VELOCITIES[(gameObject->rotation + 4) & 7]};

	// Check if supposed to move on a given axis this frame
	if ((gameObject->rotation & 3) == 2) { // If player is facing diagonally
		// Sync x and y movement counters
		if (gameObject->movement_counter.x != gameObject->movement_counter.y) {
			gameObject->movement_counter.y = gameObject->movement_counter.x;
		}

		// Perform movement only with x. y will be synced afterwards to save resources
		gameObject->movement_counter.x += gameObject_velocity.x;

		if (gameObject->movement_counter.x > 255) {
			gameObject->movement_counter.x -= 255;
			gameObject_movement->x = 1;
			gameObject_movement->y = 1;
		} else {
			gameObject_movement->x = 0;
			gameObject_movement->y = 0;
		}

		gameObject->movement_counter.y = gameObject->movement_counter.x;
	} else {
		gameObject->movement_counter.x += gameObject_velocity.x;
		if (gameObject->movement_counter.x > 255) {
			gameObject->movement_counter.x -= 255;
			gameObject_movement->x = 1;
		} else {
			gameObject_movement->x = 0;
		}

		gameObject->movement_counter.y += gameObject_velocity.y;
		if (gameObject->movement_counter.y > 255) {
			gameObject->movement_counter.y -= 255;
			gameObject_movement->y = 1;
		} else {
			gameObject_movement->y = 0;
		}
	}

	gameObject_movement->x *= gameObject->rotation < 8 ? 1 : -1;
	gameObject_movement->y *= gameObject->rotation < 12 && gameObject->rotation > 4 ? 1 : -1;
}