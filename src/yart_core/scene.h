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

    c_bsdf *get_bsdf(const c_ray& ray) const;

	diff_geom_ptr geom_dg;
	scene_object_ptr scene_obj; 
	c_transform world_to_obj;
	c_transform obj_to_world; 
	uint32_t shape_id; 
	float ray_eps; 
};
typedef boost::shared_array<c_intersection> isect_array_ptr; 

class c_scene
{

public:
	c_scene(); 

	bool query_intersection(const c_ray& ray) const; 
	bool query_intersection(const c_ray& ray, PARAM_OUT c_intersection *isect) const; 
	
	void add_light(const light_ptr light)
	{
		assert(light); 
		m_lights.push_back(light);
	}

	uint32_t get_num_lights() const { return m_lights.size(); }
	light_ptr get_light(uint32_t idx) const 
	{
		assert(idx < get_num_lights()); 
		return m_lights[idx]; 
	}
	
private:
	
	mutable scene_objects_list m_scene_objects; 
	mutable lights_list m_lights; 

};