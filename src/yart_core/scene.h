#pragma once

#include <vector>
#include "prerequisites.h"
#include "transform.h"

typedef std::vector<light_ptr> lights_list; 

class c_scene
{
public:
	c_scene(accel_structure_ptr accel_structure = accel_structure_ptr())
		: m_accel_structure(accel_structure)
	{
	}

	bool query_intersection(const c_ray& ray) const; 
	bool query_intersection(const c_ray& ray, PARAM_OUT c_intersection *isect) const; 
	
	void add_light(const light_ptr light)
	{
		assert(light); 
		m_lights.push_back(light);
	}

	uint32_t get_num_lights() const 
	{ 
		return m_lights.size(); 
	}
	
	light_ptr get_light(uint32_t idx) const 
	{
		assert(idx < get_num_lights()); 
		return m_lights[idx]; 
	}
	
private:
	
	accel_structure_ptr m_accel_structure;
	
	mutable lights_list m_lights; 

};