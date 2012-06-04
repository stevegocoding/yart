#pragma once

#include <vector>
#include "prerequisites.h"
#include "transform.h"

typedef std::vector<light_ptr> lights_array; 
typedef std::vector<triangle_mesh_ptr> meshes_array;

class c_scene
{
public:
	c_scene(accel_structure_ptr accel = accel_structure_ptr())
		: m_accel_structure(accel)
	{
	}

	c_scene(meshes_array& meshes, lights_array& lights, accel_structure_ptr accel)
		: m_meshes(meshes)
		, m_lights(lights)
		, m_accel_structure(accel)
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
	
	mutable lights_array m_lights; 
	mutable meshes_array m_meshes; 
	
};