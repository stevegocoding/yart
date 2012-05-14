#pragma once 

#include <math.h>
#include "prerequisites.h"
#include "cml/cml.h"

using namespace cml;

inline bool is_nan_vec(const vector3f& v)
{
	return ( _isnan((double)v[0]) || _isnan((double)v[1]) || _isnan((double)v[2]) ); 
}

class c_ray
{
public:
	c_ray(const vector3f& _o = vector3f(), const vector3f& _d = vector3f(), 
		  float start = 0.0f, float end = std::numeric_limits<float>::infinity(), 
		  float _t = 0.0f, 
		  int _depth = 0)
		: o(_o)
		, d(_d)
		, t_min(start) 
		, t_max(end)
	{}

	bool has_nan() const 
	{
		return ( is_nan_vec(0) || is_nan_vec(d) || _isnan(t_min) || _isnan(t_max) ); 
	}

	vector3f o;
	vector3f d;
	mutable float t_min, t_max;
};

class c_ray_differential : public c_ray
{
};