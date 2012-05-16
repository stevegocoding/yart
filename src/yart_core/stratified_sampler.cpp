#include "stratified_sampler.h"
#include "monte_carlo.h"
#include "math_utils.h"

c_stratified_sampler::c_stratified_sampler(int x_start, int x_end, int y_start, int y_end, int xs, int ys, bool jitter, float sopen, float sclose)
	: super(x_start, x_end, y_start, y_end, xs * ys, sopen, sclose)
	, m_jitter_samples(jitter)
	, m_xpos(x_start)
	, m_ypos(y_start)
	, m_x_pixel_samples(xs)
	, m_y_pixel_samples(ys)
{
	m_img_sample_buf = samples_buf_ptr(new float[2 * m_x_pixel_samples * m_y_pixel_samples]);	
	m_lens_sample_buf = samples_buf_ptr(new float[2 * m_x_pixel_samples * m_y_pixel_samples]);
	m_time_sample_buf = samples_buf_ptr(new float[m_x_pixel_samples * m_y_pixel_samples]); 
}

int c_stratified_sampler::get_current_pixel_samples(samples_array_ptr& samples, c_rng& rng)
{
	if (m_ypos == m_y_pixel_end)
		return 0; 

	int spp = m_x_pixel_samples * m_y_pixel_samples; 
	
	// Generate stratified camera samples for pixel (xpos, ypos); 
	stratified_sample_2D(m_img_sample_buf, m_x_pixel_samples, m_y_pixel_samples, rng, m_jitter_samples); 
	stratified_sample_2D(m_lens_sample_buf, m_x_pixel_samples, m_y_pixel_samples, rng, m_jitter_samples);
	stratified_sample_1D(m_time_sample_buf, m_x_pixel_samples*m_y_pixel_samples, rng, m_jitter_samples); 

	// Shift stratified image samples to pixel coordinates 
	for (int i = 0; i < spp; ++i)
	{
		m_img_sample_buf[2*i] += m_xpos; 
		m_img_sample_buf[2*i+1] += m_ypos; 
	}

	// Decorrelate the sample dimensions
	shuffle(m_lens_sample_buf, m_x_pixel_samples*m_y_pixel_samples, 2, rng); 
	shuffle(m_time_sample_buf, m_x_pixel_samples*m_y_pixel_samples, 1, rng); 
	
	// Initialize stratified _samples_ with sample values
	for (int i = 0; i < spp; ++i)
	{
		// Initialize the camera samples 
		samples[i].image_x = m_img_sample_buf[2*i];
		samples[i].image_y = m_img_sample_buf[2*i+1]; 
		samples[i].lens_u = m_lens_sample_buf[2*i]; 
		samples[i].lens_v = m_lens_sample_buf[2*i+1];
		samples[i].time = lerp(m_time_sample_buf[i], m_shutter_open, m_shutter_close); 

		// Generate additional stratified samples for integrators
		for (uint32_t j = 0; j < samples[i].num_1D(); ++j)
		{
			latin_hypercube(samples[i].get_1D_samples_buf(j), samples[i].get_1D_samples_size(j), 1, rng);			
		}
		
		for (uint32_t j = 0; j < samples[i].num_2D(); ++j)
		{
			latin_hypercube(samples[i].get_2D_samples_buf(j), samples[i].get_2D_samples_size(j), 2, rng); 
		}
	}

	// Advance to next pixel for stratified sampling
	if (++m_xpos == m_x_pixel_end)
	{
		m_xpos = m_x_pixel_start; 
		++m_ypos; 
	}
	
	return spp;
}