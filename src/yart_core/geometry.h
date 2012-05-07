#pragma once 

#include "prerequisites.h"
#include "cml/cml.h"

using namespace cml;
:
class c_ray
{
public:
	c_ray(const vector3f& _o, const vector3f& _d, 
		  float start = 0.0f, float end = std::numeric_limits<float>::infinity(), 
		  float _t = 0.0f, 
		  int _depth = 0)
		: o(_o)
		, d(_d)
		, t_min(start) 
		, t_max(end)
	{}

	vector3f o;
	vector3f d;
	mutable float t_min, t_max;
};

class c_ray_differential : public c_ray
{
	
	
	
};