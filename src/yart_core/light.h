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

struct c_occlusion_tester
{
	c_occlusion_tester(const point3f& p1, float eps1, const point3f& p2, float eps2, float time = 0.0f)
	{
		float dist = length(p2 - p1);
		ray = c_ray(p1, (p2-p1)/dist, eps1, dist*(1.0f-eps2), time); 
		assert (!ray.has_nan());
	}

	c_occlusion_tester(const c_ray& r)
		: ray(r)
	{
		assert(!ray.has_nan());
	}

	bool is_occluded(scene_ptr scene) const; 
	
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

	/** 
		Take a point on the surface and a light sample as input. 
		Returns the radiance arriving at that point, the incident direction (p->light) and the pdf value
	*/
	virtual c_spectrum sample_l(const point3f& p, float p_eps, const c_light_sample& light_sample, 
		PARAM_OUT vector3f *wi, PARAM_OUT float *pdf, PARAM_OUT c_occlusion_tester *visibility) const = 0; 

	float eval_pdf(const point3f& p, const vector3f& v) const; 
	
	virtual bool is_delta() const = 0;
	
	virtual c_spectrum compute_le(const c_ray& ray) const { return c_spectrum(0); }  

	virtual c_spectrum compute_flux() const = 0; 

protected:
	uint32_t m_num_samples;
	c_transform  m_light_to_world, m_world_to_light;
};