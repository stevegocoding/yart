#pragma once

#include "prerequisites.h"

class c_renderer
{
public:
	c_renderer(); 
	virtual ~c_renderer(); 

	/**
		Render one frame of the scene
		- initialize the integrators
		- initialize the sampler
		- enter main rendering loop 
			-	get samples for a pixel
			-	camera generates the rays through each sample 
			-	query intersections between the ray and the scene
			-*- solve the rendering equation for each ray and collect the radiance 
			-	reconstruct the image with the samples generated
			-	output the rendering result
	*/
	virtual void render_scene(scene_ptr scene) = 0;
	

	/** 
		Compute the radiance brought back by ONE ray 
	*/ 
	virtual c_spectrum render_ray(scene_ptr scene, 
		const c_ray& ray, 
		const c_sample *sample, 
		const c_rng& rng, 
		PARAM_OUT c_intersection *isect, 
		PARAM_OUT c_spectrum *l) const = 0;
	
};  