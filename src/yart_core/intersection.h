#pragma once

#include "prerequisites.h"
#include "geometry.h"
#include "transform.h"

struct c_intersection
{
	c_intersection()
		: ray_eps(0.f)
		, scene_primitive(NULL)
	{	
	}

	c_bsdf *get_bsdf(const c_ray& ray) const; 

	c_differential_geometry geom_dg;
	const c_scene_primtive *scene_primitive; 
	c_transform obj_to_world; 
	float ray_eps; 
};

typedef boost::shared_array<c_intersection> isect_array_ptr;  
