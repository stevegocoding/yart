#pragma once 

#include "prerequisites.h"
#include "sampler.h"

class c_stratified_sampler : public c_sampler
{
	typedef c_sampler super;

public:
	c_stratified_sampler(int x_start, int x_end, int y_start, int y_end, int xs, int ys, bool jitter, float sopen, float sclose);
	virtual int get_current_pixel_samples(samples_array_ptr& samples, c_rng& rng); 
	virtual int get_max_num_samples() const { return m_x_pixel_samples * m_y_pixel_samples; }
	
private:
	bool m_jitter_samples; 
	
	// Current pixel coordinates in raster space 
	int m_xpos, m_ypos; 
	
	// Number of samples in x, y per pixel
	int m_x_pixel_samples, m_y_pixel_samples; 

	samples_buf_ptr m_img_sample_buf; 
	samples_buf_ptr m_lens_sample_buf; 
	samples_buf_ptr m_time_sample_buf; 
}; 