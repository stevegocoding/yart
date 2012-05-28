#include "direct_lighting_integrator.h"
#include "geometry.h"
#include "scene.h"
#include "intersection.h"

void c_direct_lighting_integrator::request_samples(sampler_ptr sampler, c_sample *cam_sample, scene_ptr scene) 
{
	// Allocate samples 
	uint32_t n_lights = scene->get_num_lights(); 

	m_lights_samples_records = light_sample_record_array_ptr(new c_light_sample_record[n_lights]);
	m_bsdf_samples_records = bsdf_sample_record_array_ptr(new c_bsdf_sample_record[n_lights]); 	

	for (uint32_t i = 0; i < n_lights; ++i)
	{
		light_ptr l = scene->get_light(i); 
		uint32_t n_samples = l->get_num_samples(); 
		m_lights_samples_records[i] = c_light_sample_record(n_samples, cam_sample); 
		m_bsdf_samples_records[i] = c_bsdf_sample_record(n_samples, cam_sample); 
	}

}

c_spectrum c_direct_lighting_integrator::compute_li(const scene_ptr scene, 
	const c_renderer *renderer, 
	const c_ray& ray, 
	const c_intersection& isect, 
	const c_sample *sample, 
	c_rng& rng) const
{
	c_spectrum lo(0.0f); 

	// Get the BSDF at the hit point
	c_bsdf *bsdf = isect.get_bsdf(ray); 
	
	// Reverse the ray direction for shading 
	vector3f wo = -ray.d; 

	// Get the local geometry information at p
	const point3f& p = bsdf->get_shading_dg()->p;
	const vector3f& normal = bsdf->get_shading_dg()->nn; 
	
	// Compute emitted light if ray hit an area light source
	// lo += isect.le(wo);
	
	// Compute direct lighting 
	if (scene->get_num_lights() > 0)
	{
		lo += uniform_all_lights(scene, renderer, p, normal, wo, isect.ray_eps, bsdf, sample, rng, m_lights_samples_records, m_bsdf_samples_records);
	}
	
	// @TODO: Specular and transmit

	return lo;
}