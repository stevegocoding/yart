#include "direct_lighting_integrator.h"

c_spectrum c_direct_lighting_integrator::compute_li(const scene_ptr scene, 
	const renderer_ptr renderer, 
	const c_ray& ray, 
	const c_intersection& isect, 
	const sample_ptr sample, 
	c_rng& rng, 
	pool_ptr mem_pool) const
{
	

	return c_spectrum(0.0f);
}