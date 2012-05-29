#include "sampler_renderer.h"
#include "sampler.h"
#include "color.h"
#include "rng.h"
#include "ray.h"
#include "camera.h"
#include "render_target.h"
#include "scene.h"
#include "intersection.h"
#include "integrator.h"
#include "display.h"

c_sampler_renderer::c_sampler_renderer(sampler_ptr sampler, 
	camera_ptr cam, 
	surface_integrator_ptr surface_integrator, 
	volume_integrator_ptr vol_integrator, 
	c_display *display)
	: m_sampler(sampler)
	, m_camera(cam)
	, m_surface_integrator(surface_integrator)
	, m_volume_integrator(vol_integrator) 
	, m_display(display)
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
	
	ray_array_ptr rays(new c_ray[max_samples]);
	spectrum_array_ptr ls(new c_spectrum[max_samples]); 
	spectrum_array_ptr ts(new c_spectrum[max_samples]); 
	isect_array_ptr isects(new c_intersection[max_samples]); 
	samples_array_ptr samples_array = origin_sample->duplicate(max_samples); 
	

	while ((num_pixel_samples = m_sampler->get_current_pixel_samples(samples_array, rng)) > 0)
	{
		for (int j = 0; j < num_pixel_samples; ++j)
		{
			m_camera->generate_ray(samples_array[j], &rays[j]); 

			ls[j] = render_ray(scene, rays[j], &samples_array[j], rng, &isects[j], &ts[j]); 

			assert(!ls[j].has_nan());
	
			m_camera->get_render_target()->add_sample(samples_array[j], ls[j]); 
		}
	}

	m_display->update_display(m_camera->get_render_target()); 
	
}

c_spectrum c_sampler_renderer::render_ray(scene_ptr scene, 
	const c_ray& ray, 
	const c_sample *sample, 
	c_rng& rng, 
	PARAM_OUT c_intersection *isect,
	PARAM_OUT c_spectrum *l) const
{
	c_spectrum lo(0.0f); 

	if (scene->query_intersection(ray, isect))
	{
		lo = m_surface_integrator->compute_li(scene, this, ray, *isect, sample, rng);
	}
	 
	return lo; 
}