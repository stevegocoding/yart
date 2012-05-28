#pragma once 

#include <vector>
#include "prerequisites.h"
#include "scene_primitive.h"

class c_accel_structure : public c_scene_primtive 
{
public:
	c_accel_structure() {} 

	c_bsdf *get_bsdf(const c_differential_geometry& geom_dg, const c_transform& o2w) const
	{
		assert(false); 
		return NULL; 
	}

protected: 
	typedef std::vector<scene_primitive_ptr> scene_primitives_vec; 
	scene_primitives_vec m_scene_primitives;
};

//////////////////////////////////////////////////////////////////////////

class c_naive_accel_structure : public c_accel_structure
{
public:
	c_naive_accel_structure(const std::vector<scene_primitive_ptr>& prims); 

	// Linear search 
	virtual bool intersects(const c_ray& ray, c_intersection *isect) const;
	
	virtual c_aabb get_world_aabb() const; 
};

accel_structure_ptr make_naive_accel_strcture(std::vector<scene_primitive_ptr>& prims);