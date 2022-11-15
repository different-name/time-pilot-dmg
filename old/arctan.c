#include "arctan.h"
#include <stdio.h>

void main() {
	int x1 = 1;
	int y1 = 1;
	int x2 = -1;
	int y2 = -1;

	int x = (x2 - x1) * FP_DECIMAL_FACTOR;
	int y = (y2 - y1) * FP_DECIMAL_FACTOR;
	
	arctan(FP_DIV(y, x));
}

int arctan(int x) {
	int pow3 = FP_MUL(FP_MUL(x, x), x);
	int pow5 = FP_MUL(FP_MUL(pow3, x), x);
	int pow7 = FP_MUL(FP_MUL(pow7, x), x);
	int value1 = FP_DIV(pow3, 3 * FP_DECIMAL_FACTOR);
	int value2 = FP_DIV(pow5, 5 * FP_DECIMAL_FACTOR);
	int value3 = FP_DIV(pow7, 7 * FP_DECIMAL_FACTOR);
	printf("%d", pow3);
	return (FP_MUL(x - value1 + value2 - value3, 8/3.1415926 * FP_DECIMAL_FACTOR) + 8 * FP_DECIMAL_FACTOR) / FP_DECIMAL_FACTOR;
}