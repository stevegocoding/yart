#pragma once

#include "prerequisites.h"
#include "integrator.h"
#include "light.h"
#include "reflection.h"

class c_direct_lighting_integrator : public c_surface_integrator
{
public:
	c_direct_lighting_integrator() {} 

	virtual void request_samples(sampler_ptr sampler, c_sample *cam_sample, scene_ptr scene); 

	virtual c_spectrum compute_li(const scene_ptr scene, 
		const renderer_ptr renderer, 
		const c_ray& ray, 
		const c_intersection& isect, 
		const c_sample *sample, 
		c_rng& rng,
		pool_ptr mem_pool) const; 


private: 
	
	light_sample_record_array_ptr m_lights_samples_records; 
	bsdf_sample_record_array_ptr m_bsdf_samples_records; 
	
	
};