/* __rem_pio2f(x,y)
 *
 * return the remainder of x rem pi/2 in *y
 * use double precision for everything except passing x
 * use __rem_pio2_large() for large x
 */

#include <math.h>

#define EPS DBL_EPSILON
/*
 * invpio2:  53 bits of 2/pi
 * pio2_1:   first 25 bits of pi/2
 * pio2_1t:  pi/2 - pio2_1
 */
static const double
toint   = 1.5/EPS,
invpio2 = 6.36619772367581382433e-01, /* 0x3FE45F30, 0x6DC9C883 */
pio2_1  = 1.57079631090164184570e+00, /* 0x3FF921FB, 0x50000000 */
pio2_1t = 1.58932547735281966916e-08; /* 0x3E5110b4, 0x611A6263 */

int __rem_pio2f(float x, double *y)
{
	union {float f; uint32_t i;} u = {x};
	double tx[1],ty[1];
	double_t fn;
	uint32_t ix;
	int n, sign, e0;

	ix = u.i & 0x7fffffff;
	/* 25+53 bit pi is good enough for medium size */
	if (ix < 0x4dc90fdb) {  /* |x| ~< 2^28*(pi/2), medium size */
		/* Use a specialized rint() to get fn.  Assume round-to-nearest. */
		fn = (double_t)x*invpio2 + toint - toint;
		n  = (int32_t)fn;
		*y = x - fn*pio2_1 - fn*pio2_1t;
		return n;
	}
	if(ix>=0x7f800000) {  /* x is inf or NaN */
		*y = x-x;
		return 0;
	}
	/* scale x into [2^23, 2^24-1] */
	sign = u.i>>31;
	e0 = (ix>>23) - (0x7f+23);  /* e0 = ilogb(|x|)-23, positive */
	u.i = ix - (e0<<23);
	tx[0] = u.f;
	n  =  __rem_pio2_large(tx,ty,e0,1,0);
	if (sign) {
		*y = -ty[0];
		return -n;
	}
	*y = ty[0];
	return n;
}
