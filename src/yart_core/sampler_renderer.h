#pragma once 

#include "prerequisites.h"
#include "renderer.h"
#include "renderer_report.h"

class c_sampler_renderer : public c_renderer
{
public:
	c_sampler_renderer(sampler_ptr sampler, 
		camera_ptr cam, 
		surface_integrator_ptr surface_integrator, 
		volume_integrator_ptr vol_integrator, 
		c_display *display);
	~c_sampler_renderer(); 

	virtual void render_scene(scene_ptr scene); 

	virtual c_spectrum render_ray(scene_ptr scene, 
		const c_ray& ray, 
		const c_sample *sample, 
		c_rng& rng, 
		PARAM_OUT c_intersection *isect,
		PARAM_OUT c_spectrum *l) const;

	renderer_report_ptr get_report() const { return m_report; }
	
private:
	
	sampler_ptr m_sampler;
	camera_ptr m_camera; 
	surface_integrator_ptr m_surface_integrator;
	volume_integrator_ptr m_volume_integrator; 
	c_display *m_display;

	renderer_report_ptr m_report; 
};