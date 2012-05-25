#pragma once 

#include "prerequisites.h" 
#include "color.h"

class c_integrator
{
public:
	virtual ~c_integrator() {} 
	
	virtual void request_samples(sampler_ptr sampler, c_sample *sample, scene_ptr scene) {}
};

class c_surface_integrator : public c_integrator
{
public:
	virtual c_spectrum compute_li(const scene_ptr scene, 
		const renderer_ptr renderer, 
		const c_ray& ray, 
		const c_intersection& isect, 
		const sample_ptr sample, 
		c_rng& rng,
		pool_ptr mem_pool) const = 0; 
};

class c_volume_integrator : public c_integrator
{
	
};