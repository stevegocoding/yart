#pragma once

#include "prerequisites.h"

class c_renderer
{
public:
	c_renderer(); 
	virtual ~c_renderer(); 

	virtual void render_scene(scene_ptr scene) = 0;
	
	virtual c_spectrum compute_li(scene_ptr scene, const c_ray& ray, sample_ptr sample, const c_rng& rng, c_intersection *isect) const = 0;
	
};

