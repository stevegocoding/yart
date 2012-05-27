#pragma once 

#include "prerequisites.h" 
#include "transform.h"
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
		const c_renderer *renderer, 
		const c_ray& ray, 
		const c_intersection& isect, 
		const c_sample *sample, 
		c_rng& rng) const = 0; 
};

class c_volume_integrator : public c_integrator
{
	
};


c_spectrum uniform_all_lights(const scene_ptr scene, 
	const c_renderer *renderer, 
	const point3f& p, 
	const vector3f& normal,
	const vector3f& wo, 
	float ray_eps,
	const c_bsdf *bsdf,
	const c_sample *sample,
	c_rng& rng,
	const light_sample_record_array_ptr light_samples_record,
	const bsdf_sample_record_array_ptr bsdf_samples_record); 