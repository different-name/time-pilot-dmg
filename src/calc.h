#ifndef CALC_H
#define CALC_H

#include <gb/gb.h>

#include "main.h"

#define abs(x) ((x) < 0 ? -(x) : (x))

int8_t direction_to_point(Point p1, Point p2);

int8_t step_to_rotation(int8_t r1, int8_t r2);

Vector2D velocity_from_rotation_and_distance(int8_t rotation, uint16_t distance);

#endif