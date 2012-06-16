#pragma once 

#include <math.h> 
#include <algorithm>

#include "cml/cml.h"

using std::swap;
using std::sqrt;
using namespace cml;

namespace 
{
	enum e_vec_component
	{
		x = 0, 
		y = 1, 
		z = 2,
		w = 3
	};

	static const float INV_PI = 0.31830988618379067154f; 
    static const float INV_TWOPI = 0.15915494309189533577f; 
	static const float INFINITY = std::numeric_limits<float>::infinity();
}

inline bool quadratic(float A, float B, float C, float *t0, float *t1)
{
    // Find quadratic discriminant
    float discrim = B * B - 4.f * A * C;
    if (discrim <= 0.) 
        return false;
    float rootDiscrim = sqrtf(discrim);

    // Compute quadratic _t_ values
    float q;
    if (B < 0) 
        q = -.5f * (B - rootDiscrim);
    else       
        q = -.5f * (B + rootDiscrim);
    *t0 = q / A;
    *t1 = C / q;
    if (*t0 > *t1) swap(*t0, *t1);
    return true;
    
}

inline float lerp(float t, float v1, float v2)
{
	return (1.0f - t) * v1 + t * v2; 
}

inline int clamp(int val, int low, int high)
{
    if (val < low) return low; 
    else 
        if (val > high) 
            return high; 
        else
            return val;
}

inline float clamp(float val, float low, float high)
{
    if (val < low) return low; 
    else 
        if (val > high) 
            return high; 
        else
            return val;
}

inline int floor2int(float val)
{
	return (int)floorf(val); 
}

inline int round2int(float val)
{
	return floor2int(val + 0.5f); 
}

inline float log2(float x)
{
	static float inv_log2 = 1.f / logf(2.0f);
	return logf(x) * inv_log2; 
}

inline int log2int(float val)
{
	return floor2int(log2(val));
}

bool solve_linear_system2x2(const float A[2][2], 
	const float B[2], float *x0, float *x1);