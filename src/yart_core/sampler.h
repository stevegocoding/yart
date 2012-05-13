#pragma once

#include <vector>
#include "prerequisites.h"

class c_sampler 
{
public:
    c_sampler(int x_start, int x_end, int y_start, int y_end, int spp, float sopen, float sclose);
	
	virtual int get_current_pixel_samples(samples_array_ptr& sample, c_rng& rng) = 0;
	virtual int get_max_num_samples() const = 0; 

protected: 
	int m_x_pixel_start, m_x_pixel_end; 
	int m_y_pixel_start, m_y_pixel_end;
	int m_sample_per_pixel;
	float m_shutter_open, m_shutter_close; 
}; 

struct c_camera_sample
{ 
	float image_x, image_y;
	float lens_u, lens_v; 
	float time;
	c_camera_sample()
		: image_x(0), image_y(0)
		, lens_u(0), lens_v(0)
		, time(0)
	{}
};

struct c_sample : public c_camera_sample
{ 
public:
	c_sample() {} 
	c_sample(sampler_ptr sampler, surface_integrator_ptr surface_itegrator, volume_integrator_ptr vol_integrator, scene_ptr& scene); 
	~c_sample() 
	{
		delloc_samples_memory(); 
	}
	  
	int add_1D(uint32_t num_samples); 
	int add_2D(uint32_t num_samples); 
	
	void set_1D(const std::vector<uint32_t>& _1D) {m_1D = _1D;}
	void set_2D(const std::vector<uint32_t>& _2D) {m_2D = _2D;}
	samples_array_ptr duplicate(uint32_t num_samples); 
	
	uint32_t num_1D() const { return m_1D.size(); }
	uint32_t num_2D() const { return m_2D.size(); }
	
	uint32_t get_1D_samples_size(uint32_t idx) const 
	{ 
		return m_1D[idx]; 
	}
	
	uint32_t get_2D_samples_size(uint32_t idx) const
	{
		return m_2D[idx];
	}

	samples_buf_ptr get_1D_samples_buf(uint32_t idx) const 
	{ 
		return m_1D_samples[idx]; 
	}

	samples_buf_ptr get_2D_samples_buf(uint32_t idx) const 
	{ 
		return m_2D_samples[idx]; 
	}
	
private:
	void alloc_samples_memory();
	void delloc_samples_memory(); 
	
	// Number of samples of each 1D samples patterns (addition samples to camera samples)
	std::vector<uint32_t> m_1D;
	// Number of samples of each 2D samples patterns (addition samples to camera samples)
	std::vector<uint32_t> m_2D;
	
	std::vector<samples_buf_ptr> m_1D_samples;
	std::vector<samples_buf_ptr> m_2D_samples; 
}; 