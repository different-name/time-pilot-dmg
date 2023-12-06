#ifndef CALC_H
#define CALC_H

#include <gb/gb.h>

#include "main.h"

#define abs(x) ((x) < 0 ? -(x) : (x))

uint8_t direction_to_point(UVector8* p1, UVector8* p2);

uint8_t step_to_rotation(int8_t r1, int8_t r2);

void movement_from_velocity(GameObject* gameObject, Vector8* movement);

#endif