#include "common.h"

unsigned gcd(unsigned a, unsigned b)
{
	if(b == 0) return a;
	return gcd(b, a % b);
}