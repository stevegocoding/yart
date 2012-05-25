#pragma once 

#include "prerequisites.h"
#include "transform.h"
#include "color.h"

struct light_smaple_offset
{
	
};

struct c_light_sample
{
	c_light_sample() {}
	c_light_sample(const sample_ptr& sample);
	float u_pos[2], u_component;  
};

struct c_visibility_tester
{
	

	c_ray ray; 
};

class c_light 
{

public: 
	c_light(const c_transform& light_to_world, uint32_t num_samples = 1)
		: m_num_samples(num_samples)
		, m_light_to_world(light_to_world)
		, m_world_to_light(inverse_transform(light_to_world))
	{}

	virtual c_spectrum sample_l(const point3f& p, float p_eps, const c_light_sample& light_sample, float time,
		PARAM_OUT vector3f *wi, PARAM_OUT float *pdf, ) const = 0;

protected:
	uint32_t m_num_samples;
	c_transform  m_light_to_world, m_world_to_light;
};