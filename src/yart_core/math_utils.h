#pragma once 

#include <math.h> 
#include <algorithm>

using std::swap;
using std::sqrt;

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