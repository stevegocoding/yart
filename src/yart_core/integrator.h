#pragma once 

#include "prerequisites.h"

class c_integrator
{
public:
	virtual ~c_integrator() {} 
	
	virtual void request_samples(sampler_ptr sampler, c_sample *sample, scene_ptr scene) {}
};

class c_surface_integrator : public c_integrator
{
	
};

class c_volume_integrator : public c_integrator
{
	
};