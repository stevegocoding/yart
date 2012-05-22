#include "rng.h"

// Random Number Method Definitions
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

void c_rng::seed(uint32_t _seed) const {
	mt[0]= _seed & 0xffffffffUL;
	for (mti=1; mti<n; mti++) {
		mt[mti] =
			(1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
		/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
		/* In the previous versions, MSBs of the seed affect   */
		/* only MSBs of the array mt[].                        */
		/* 2002/01/09 modified by Makoto Matsumoto             */
		mt[mti] &= 0xffffffffUL;
		/* for >32 bit machines */
	}
}

/* generates a random number on [0,1)-real-interval */
float c_rng::random_float() const
{
	float v = (random_uint() & 0xffffff) / float(1 << 24);
	return v;
}


// Random Number Functions
unsigned long c_rng::random_uint() const
{
	unsigned long y;
	static unsigned long mag01[2]={0x0UL, MATRIX_A};
	/* mag01[x] = x * MATRIX_A  for x=0,1 */

	if (mti >= n) { /* generate N words at one time */
		// PBRT_RNG_STARTED_TABLEGEN();
		int kk;

		if (mti == n+1)   /* if Seed() has not been called, */
			seed(5489UL); /* default initial seed */

		for (kk=0;kk<n-M;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		for (;kk<n-1;kk++) {
			y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
			mt[kk] = mt[kk+(M-n)] ^ (y >> 1) ^ mag01[y & 0x1UL];
		}
		y = (mt[n-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
		mt[n-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

		mti = 0;
		// PBRT_RNG_FINISHED_TABLEGEN();
	}

	y = mt[mti++];

	/* Tempering */
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= (y >> 18);

	return y;
}