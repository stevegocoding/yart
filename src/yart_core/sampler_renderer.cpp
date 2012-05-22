#include "sampler_renderer.h"
#include "sampler.h"
#include "color.h"
#include "rng.h"
#include "ray.h"
#include "camera.h"
#include "render_target.h"


c_sampler_renderer::c_sampler_renderer(sampler_ptr sampler, camera_ptr cam, surface_integrator_ptr surface_integrator, volume_integrator_ptr vol_integrator, bool vis_ids)
	: m_sampler(sampler)
	, m_camera(cam)
	, m_surface_integrator(surface_integrator)
	, m_volume_integrator(vol_integrator) 
{
	
}

c_sampler_renderer::~c_sampler_renderer()
{
	
}


void c_sampler_renderer::render_scene(scene_ptr scene)
{
	

	//////////////////////////////////////////////////////////////////////////
	// Allocate and initialize sample 
	//////////////////////////////////////////////////////////////////////////
	sample_ptr origin_sample = sample_ptr(new c_sample(m_sampler, m_surface_integrator, m_volume_integrator, scene));

	c_rng rng(2047); 

	int num_pixel_samples = 0; 
	int max_samples = m_sampler->get_max_num_samples(); 
	samples_array_ptr samples_array = origin_sample->duplicate(max_samples); 
	spectrum_array_ptr ls(new c_spectrum[max_samples]); 
	while ((num_pixel_samples = m_sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (int j = 0; j < num_pixel_samples; ++j)
		{
			c_ray r; 
			m_camera->generate_ray(samples_array[j], &r); 

			scene->

			// ls[j] = c_spectrum(red, green, blue); 
			m_camera->get_render_target()->add_sample(samples_array[j], ls[j]);
		}
	} 
	
	
}