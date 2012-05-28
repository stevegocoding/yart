#pragma once 

#include "prerequisites.h"
#include "geometry.h" 
#include "transform.h"

class c_shape
{
public:
    c_shape()
	{}
    
    virtual ~c_shape() {}

    virtual bool intersects(const c_ray& ray, 
        PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT c_differential_geometry *geom_dg) const { return false; }

	virtual c_aabb get_world_aabb(const c_transform& o2w) const { return c_aabb(); } 

	virtual void apply_transform(const c_transform& t) {} 

	virtual void get_shading_geometry(const c_transform& o2w, 
		const c_differential_geometry& diff_geom, 
		PARAM_OUT c_differential_geometry *shading_dg) const 
	{
		*shading_dg = diff_geom; 
	} 
};