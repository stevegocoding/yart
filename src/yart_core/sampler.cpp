#include "sampler.h"
#include "integrator.h"
#include "boost/foreach.hpp"

c_sampler::c_sampler(int x_start, int x_end, int y_start, int y_end, int spp, float sopen, float sclose)
	: m_x_pixel_start(x_start)
	, m_x_pixel_end(x_end)
	, m_y_pixel_start(y_start)
	, m_y_pixel_end(y_end)
	, m_sample_per_pixel(spp)
	, m_shutter_open(sopen)
	, m_shutter_close(sclose)
{
	
}

//////////////////////////////////////////////////////////////////////////

c_sample::c_sample(sampler_ptr sampler, surface_integrator_ptr surface_itegrator, volume_integrator_ptr vol_integrator, scene_ptr& scene)
{
	if (surface_itegrator)
		surface_itegrator->request_samples(sampler, this, scene);
	if (vol_integrator)
		vol_integrator->request_samples(sampler, this, scene);

	alloc_samples_memory(); 
}

void c_sample::alloc_samples_memory()
{
	// Calculate the total samples for 1D and 2D samples
	uint32_t total_1D = 0; 
	BOOST_FOREACH(uint32_t n, m_1D)
	{
		total_1D += n; 
	}
	
	uint32_t total_2D = 0; 
	BOOST_FOREACH(uint32_t n, m_2D)
	{
		total_2D += 2 * n;			// two-dimension sampling  
	}
	
	// Allocate memory
	BOOST_FOREACH(uint32_t n, m_1D)
	{
		boost::shared_array<float> array = boost::shared_array<float>(new float[total_1D]);
		m_1D_samples.push_back(array);
	}
	
	BOOST_FOREACH(uint32_t n, m_2D)
	{
		boost::shared_array<float> array = boost::shared_array<float>(new float[total_2D]);
		m_2D_samples.push_back(array);
	}
}

void c_sample::delloc_samples_memory()
{
	BOOST_FOREACH(boost::shared_array<float> p_array, m_1D_samples)
	{
		p_array.reset();
	}

	BOOST_FOREACH(boost::shared_array<float> p_array, m_2D_samples)
	{
		p_array.reset();
	}
}

int c_sample::add_1D(uint32_t num_samples)
{
	m_1D.push_back(num_samples); 
	return m_1D.size() - 1;
}

int c_sample::add_2D(uint32_t num_samples)
{
	m_1D.push_back(num_samples); 
	return m_2D.size() - 1; 
}


samples_array_ptr c_sample::duplicate(uint32_t num_samples)
{
	samples_array_ptr samples = samples_array_ptr(new c_sample[num_samples]);
	for (uint32_t i = 0; i < num_samples; ++i)
	{
		samples[i].set_1D(m_1D); 
		samples[i].set_2D(m_2D); 
		samples[i].alloc_samples_memory();
	}
	
	return samples; 
}