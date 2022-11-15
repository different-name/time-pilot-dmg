#include <stdio.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define ABS(x) (((x) < 0) ? -(x) : (x))

#define FP_DECIMAL_FACTOR 100

#define FP_LIT(x) ((int)((x) * FP_DECIMAL_FACTOR))
#define FP_INT_PART(x) (((x) + 50) / FP_DECIMAL_FACTOR)

#define FP_MUL(x, y) ((x) * (y) / FP_DECIMAL_FACTOR)
#define FP_DIV(x, y) ((x) * FP_DECIMAL_FACTOR / (y))

int angle_between_pos(int startX, int startY, int targetX, int targetY) {
	if (startX == targetX) {
		return (startY < targetY) * 8;
	}
	if (startY == targetY) {
		return (startX > targetX) * 8 + 4;
	}

	int x = FP_LIT(targetX - startX);
	int y = FP_LIT(targetY - startY);

	int a = FP_DIV(MIN(ABS(x), ABS(y)), MAX(ABS(x), ABS(y)));
	int s = FP_MUL(a, a);
	int r = FP_MUL(FP_MUL((FP_MUL((FP_MUL(-5, s) + 16), s) - 32), s), a) + a;

	if (ABS(y) > ABS(x)) {
		r = 157 - r;
	}
	if (x < 0) {
		r = 314 - r;
	}
	if (y < 0) {
		r = -r;
	}

	printf("%d\n", r);

	r = r - 157;

	if (r > 0){
		r = 628 - r;
	}
	if (r < 0) {
		r = -r;
	}

	printf("%d\n", r);


	r = ((((((r + 5) / 10) * (25) / 10)) + 5) / 10);

	return r;
}

void main() {
	printf("%d\n", angle_between_pos(103, 62, 0, 0));
}

// https://stackoverflow.com/questions/55281199/represent-decimal-numbers-using-integers
// https://stackoverflow.com/questions/35183929/approximating-atan-without-a-math-library