#ifndef COMMON_H
#define COMMON_H 1

#include <math.h>

#define RAND_EXEC_TIME 1

#define EPSILON 0.00001
#define DOUBLE_EQ(a, b) (fabs(a - b) < EPSILON)

int gcd(int a, int b);

#endif /* COMMON_H */