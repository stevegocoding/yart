#pragma once 

#include "../../yart_core/integrator.h"

class c_dummy_integrator : public c_surface_integrator
{
public:
	virtual void request_samples(sampler_ptr sampler, c_sample *sample, scene_ptr scene)
	{
		// sample->add_2D(8); 
	}
};