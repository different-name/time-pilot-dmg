#ifndef CALC_H
#define CALC_H

#include <gb/gb.h>
#include "main.h"

#define FP_DIV(x, y) ((x) * FP_DECIMAL_FACTOR / (y))

uint8_t angle_between_pos(Pos start, Pos target);

#endif