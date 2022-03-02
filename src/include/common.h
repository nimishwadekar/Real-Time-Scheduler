#ifndef COMMON_H
#define COMMON_H 1

#include <math.h>

// Macro Flag: Comment out to disable randomisation of execution times.
#define RAND_EXEC_TIME 1


#define SCHEDULER_TIMESTEP_PRECISION 100
#define SCHEDULER_MINIMUM_TIMESTEP (1.0 / SCHEDULER_TIMESTEP_PRECISION)

// Truncates the passed double value to the precision of the scheduler.
#define TO_SCHEDULER_PRECISION(n) (((double) (long) (n * SCHEDULER_TIMESTEP_PRECISION)) * SCHEDULER_MINIMUM_TIMESTEP)

#define EPSILON 0.00001

// Checks if two double values are equal.
#define DOUBLE_EQ(a, b) (fabs(a - b) < EPSILON)

// Returns the GCD of a and b.
int gcd(int a, int b);

#endif /* COMMON_H */