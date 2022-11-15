#ifndef MAIN_H
#define MAIN_H


#define FP_DECIMAL_FACTOR 10

#define FP_MUL(x, y) ((x) * (y) / FP_DECIMAL_FACTOR)
#define FP_DIV(x, y) ((x) * FP_DECIMAL_FACTOR / (y))

int arctan(int x);

#endif