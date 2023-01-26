#include "calc.h"
#include "main.h"

// uint8_t angle_between_pos(Pos start, Pos target) {
// 	if (start.x == target.x) {
// 		return (start.y < target.y) * 8;
// 	}
// 	if (start.y == target.y) {
// 		return (start.x > target.x) * 8 + 4;
// 	}

// 	int16_t x = FP_LIT((int16_t)target.x - (int16_t)start.x);
// 	int16_t y = -FP_LIT((int16_t)target.y - (int16_t)start.y);

// 	int16_t a = FP_DIV((int32_t)MIN(ABS(x), ABS(y)), (int32_t)MAX(ABS(x), ABS(y)));
// 	int16_t s = FP_MUL(a, a);
// 	int16_t r = FP_MUL(FP_MUL((FP_MUL((FP_MUL(-5, s) + 16), s) - 32), s), a) + a;

// 	if (ABS(y) > ABS(x)) {
// 		r = 157 - r;
// 	}
// 	if (x < 0) {
// 		r = 314 - r;
// 	}
// 	if (y < 0) {
// 		r = -r;
// 	}

// 	r = r - 157;

// 	if (r > 0) {
// 		r = 628 - r;
// 	}
// 	if (r < 0) {
// 		r = -r;
// 	}

// 	return (((r + 5) / 10 * 25 / 10 + 5) / 10) % 16;
// }

uint8_t angle_between_pos(Pos start, Pos target) {
	if (start.x == target.x) {
		return (start.y < target.y) * 8;
	}
	if (start.y == target.y) {
		return (start.x > target.x) * 8 + 4;
	}

	if (target.x > start.x) {
		if (target.y > start.y) {
			uint8_t rise = target.y - start.y;
			uint8_t run = target.x - start.x;
			uint16_t slope = rise * 100 / run;

			if (slope > 67) {
				return (slope > 503) * 1 + (slope > 144) * 1 + 6;
			} else {
				return (slope > 10) * 1 + 4;
			}

		} else {
			uint8_t rise = start.y - target.y;
			uint8_t run = target.x - start.x;
			uint16_t slope = rise * 100 / run;

			if (slope > 67) {
				return (slope <= 503) * 1 + (slope <= 144) * 1;
			} else {
				return (slope <= 20) * 1 + 3;
			}
		}

	} else {
		if (target.y > start.y) {
			uint8_t rise = target.y - start.y;
			uint8_t run = start.x - target.x;
			uint16_t slope = rise * 100 / run;

			if (slope > 67) {
				return (slope <= 503) * 1 + (slope <= 144) * 1 + 8;
			} else {
				return (slope <= 20) * 1 + 11;
			}
			
		} else {
			uint8_t rise = start.y - target.y;
			uint8_t run = start.x - target.x;
			uint16_t slope = rise * 100 / run;

			if (slope > 67) {
				return ((slope > 503) * 1 + (slope > 144) * 1 + 14) % 16;
			} else {
				return (slope > 20) * 1 + 12;
			}
		}
	}
}