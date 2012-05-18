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
        PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const { return false; }

	virtual aabb_ptr calc_local_aabb() const { return aabb_ptr(); }

	virtual void apply_transform(const c_transform& t) {} 
};