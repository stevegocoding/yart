#pragma once 

#include "prerequisites.h"
#include "renderer.h"

class c_sampler_renderer : public c_renderer
{
public:
	c_sampler_renderer(sampler_ptr sampler, camera_ptr cam, surface_integrator_ptr surface_integrator, volume_integrator_ptr vol_integrator, bool vis_ids);
	~c_sampler_renderer(); 

	virtual void render_scene(scene_ptr scene); 

	virtual c_spectrum compute_li(scene_ptr scene, const c_ray& ray, sample_ptr sample, const c_rng& rng, c_intersection *isect) const;
	


private:
	
	sampler_ptr m_sampler;
	camera_ptr m_camera; 
	surface_integrator_ptr m_surface_integrator;
	volume_integrator_ptr m_volume_integrator; 
};