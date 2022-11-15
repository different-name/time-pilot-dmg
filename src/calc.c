#include "calc.h"
#include "main.h"

uint8_t angle_between_pos(Pos start, Pos target) {
	if (start.x == target.x) {
		return (start.y < target.y) * 8;
	}
	if (start.y == target.y) {
		return (start.x > target.x) * 8 + 4;
	}

	int16_t x = FP_LIT((int16_t)target.x - (int16_t)start.x);
	int16_t y = -FP_LIT((int16_t)target.y - (int16_t)start.y);

	int16_t a = FP_DIV((int32_t)MIN(ABS(x), ABS(y)), (int32_t)MAX(ABS(x), ABS(y)));
	int16_t s = FP_MUL(a, a);
	int16_t r = FP_MUL(FP_MUL((FP_MUL((FP_MUL(-5, s) + 16), s) - 32), s), a) + a;

	if (ABS(y) > ABS(x)) {
		r = 157 - r;
	}
	if (x < 0) {
		r = 314 - r;
	}
	if (y < 0) {
		r = -r;
	}

	r = r - 157;

	if (r > 0) {
		r = 628 - r;
	}
	if (r < 0) {
		r = -r;
	}

	return (((r + 5) / 10 * 25 / 10 + 5) / 10) % 16;
}