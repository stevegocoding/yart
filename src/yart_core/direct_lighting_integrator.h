#pragma once

#include "prerequisites.h"
#include "integrator.h"

class c_direct_lighting_integrator : public c_surface_integrator
{
public:
	c_direct_lighting_integrator() {} 

	virtual c_spectrum compute_li(const scene_ptr scene, 
		const renderer_ptr renderer, 
		const c_ray& ray, 
		const c_intersection& isect, 
		const sample_ptr sample, 
		c_rng& rng,
		pool_ptr mem_pool) const; 


private: 

	
	
};