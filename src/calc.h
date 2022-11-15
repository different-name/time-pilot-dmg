#ifndef CALC_H
#define CALC_H

#include <gb/gb.h>
#include "main.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) (((x) < 0) ? -(x) : (x))

#define FP_DECIMAL_FACTOR 100

#define FP_LIT(x) ((int)((x) * FP_DECIMAL_FACTOR))
#define FP_INT_PART(x) (((x) + 50) / FP_DECIMAL_FACTOR)

#define FP_MUL(x, y) ((x) * (y) / FP_DECIMAL_FACTOR)
#define FP_DIV(x, y) ((x) * FP_DECIMAL_FACTOR / (y))

uint8_t angle_between_pos(Pos start, Pos target);

#endif