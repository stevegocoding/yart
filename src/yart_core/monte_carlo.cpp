#include "monte_carlo.h"
#include "transform.h"

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
	float *p = &(sample_buf[0]);
	for (int y = 0; y < num_samples_y; ++y)
		for (int x = 0; x < num_samples_x; ++x)
		{
			float jx = jitter ? rng.random_float() : 0.5f;
			float jy = jitter ? rng.random_float() : 0.5f; 
			*p++ = std::min( (x + jx) * dx, OneMinusEpsilon ); 
			*p++ = std::min( (y + jy) * dy, OneMinusEpsilon ); 
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

void concentric_sample_disk(float u1, float u2, float *dx, float *dy)
{
    float r, theta; 
    
    // Map uniform random numbers to [-1, 1]^2 
    float sx = 2 * u1 - 1; 
    float sy = 2 * u2 - 1; 
    
    // Map square to (r, theta) 
    
    // Hanlde origin point 
    if (sx == 0.0f && sy == 0.0f)
    {
        *dx = 0.0f; 
        *dy = 0.0f; 
        return ; 
    }
    
    if (sx >= -sy) {     // region 1 and 2
        if (sx > sy) {   // region 1 
            r = sx;
            if (sy > 0.0) theta = sy/r;
            else          theta = 8.0f + sy/r;
        }
        else {           // region 2
            r = sy;
            theta = 2.0f - sx/r;
        }
    }
    else {                  // region 3 and 4
        if (sx <= sy) {     // region 3
            r = -sx;
            theta = 4.0f - sy/r;
        }
        else {              // region 4 
            r = -sy;
            theta = 6.0f + sx/r;
        }
    }

    theta *= M_PI / 4.f;
    *dx = r * cosf(theta);
    *dy = r * sinf(theta);   
}

vector3f cosine_sample_hemisphere(float u1, float u2)
{
    // ret vector is in hemisphere shading coordinate space
    vector3f ret; 
    concentric_sample_disk(u1, u2, &ret[0], &ret[1]); 
    ret[2] = sqrtf(max(0.0f, 1.0f - ret[0] * ret[0] - ret[1] * ret[1]));

    return ret; 
}

vector3f uniform_sample_hemisphere(float u1, float u2)
{
    float z = u1; 
    float r = sqrtf(max(0.0f, 1.0f-z*z)); 
    float phi = 2 * M_PI * u2; 
    float x = r * cosf(phi);
    float y = r * sinf(phi);
    return vector3f(x, y, z); 
}