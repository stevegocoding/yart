#include "monte_carlo.h"


void shuffle(samples_buf_ptr& sample_buf, uint32_t num_samples, uint32_t num_dims, c_rng& rng)
{
	for (uint32_t i = 0; i < num_samples; ++i)
	{
		uint32_t other = i + (rng.random_uint() % (num_samples - i)); 
		for (uint32_t j = 0; j < num_dims; ++j)
		{
			std::swap(sample_buf[num_dims*i+j], sample_buf[num_dims*other+j]); 
		}
	}
}

void stratified_sample_1D(samples_buf_ptr& sample_buf, int num_samples, c_rng& rng, bool jitter)
{
	assert(sample_buf); 
	
	float inv_num = 1.0f / num_samples; 
	for (int i = 0; i < num_samples; ++i)
	{
		float delta = jitter ? rng.random_float() : 0.5f; 
		sample_buf[i] = std::min((i +  delta) * inv_num, OneMinusEpsilon);
	} 
}

void stratified_sample_2D(samples_buf_ptr& sample_buf, int num_samples_x, int num_samples_y, c_rng& rng, bool jitter)
{
	float dx = 1.0f / num_samples_x; 
	float dy = 1.0f / num_samples_y; 
	
	for (int y = 0; y < num_samples_y; ++y)
		for (int x = 0; x < num_samples_x; x+=2)
		{
			float jx = jitter ? rng.random_float() : 0.5f;
			float jy = jitter ? rng.random_float() : 0.5f; 
			sample_buf[y * num_samples_x + x] = std::min( (x + jx) * dx, OneMinusEpsilon ); 
			sample_buf[y * num_samples_x + x + 1] = std::min( (y + jy) * dy, OneMinusEpsilon ); 
		} 
}

void latin_hypercube(samples_buf_ptr& sample_buf, uint32_t num_samples, uint32_t num_dims, c_rng& rng)
{
	// Generate LHS samples along diagonal
	float delta = 1.0f / num_samples;
	for (uint32_t i = 0; i < num_samples; ++i)
		for (uint32_t j = 0; j < num_dims; ++j)
			sample_buf[num_dims*i+j] = std::min((i+rng.random_float())*delta, OneMinusEpsilon);

	// Permute LHS samples in each dimension
	for (uint32_t i = 0; i < num_dims; ++i)
	{
		for (uint32_t j = 0; j < num_samples; ++j)
		{
			uint32_t other = j + (rng.random_uint() % (num_samples-j)); 
			std::swap(sample_buf[num_dims*j+i], sample_buf[num_dims*other+i]);
		}
	}
}