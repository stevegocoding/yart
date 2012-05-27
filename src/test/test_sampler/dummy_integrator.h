#pragma once 

#include "integrator.h"
#include "reflection.h"
#include "light.h"
#include "scene.h"

class c_dummy_integrator : public c_surface_integrator
{
public:
	virtual void request_samples(sampler_ptr sampler, c_sample *cam_sample, scene_ptr scene)
	{ 
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

	virtual c_spectrum compute_li(const scene_ptr scene, 
		const c_renderer *renderer, 
		const c_ray& ray, 
		const c_intersection& isect, 
		const c_sample *sample, 
		c_rng& rng) const
	{
		return c_spectrum(0.0f); 
	}

	light_sample_record_array_ptr m_lights_samples_records; 
	bsdf_sample_record_array_ptr m_bsdf_samples_records; 
};