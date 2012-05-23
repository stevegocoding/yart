#pragma once

#include <vector>
#include "prerequisites.h"
#include "transform.h"

typedef std::vector<scene_object_ptr> scene_objects_list; 
typedef std::vector<light_ptr> lights_list; 

struct c_intersection
{
	c_intersection()
		: shape_id(0)
		, ray_eps(0.f)
	{
		
	}

	
	
	
	diff_geom_ptr dg;
	scene_object_ptr scene_obj; 
	c_transform world_to_obj;
	c_transform obj_to_world; 
	uint32_t shape_id; 
	float ray_eps; 
};


class c_scene
{

public:
	c_scene(); 

	bool query_intersection(const c_ray& ray, PARAM_OUT c_intersection *isect) const; 
	
private:
	
	mutable scene_objects_list m_scene_objects; 
	mutable lights_list m_lights; 

};