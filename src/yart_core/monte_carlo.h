#pragma once

#include "prerequisites.h"
#include "rng.h"

// smallest floating point value less than one; all canonical random samples
// should be <= this.
// sadly, MSVC2008 (at least) doesn't support hexidecimal fp constants...
static const float OneMinusEpsilon = 0.9999999403953552f;

void shuffle(samples_buf_ptr& sample_buf, uint32_t num_samples, uint32_t num_dims, c_rng& rng);

void stratified_sample_1D(samples_buf_ptr& sample_buf, int num_samples, c_rng& rng, bool jitter); 
void stratified_sample_2D(samples_buf_ptr& sample_buf, int num_samples_x, int num_samples_y, c_rng& rng, bool jitter); 

// Latin-hypercube (n-rooks) sampling
void latin_hypercube(samples_buf_ptr& sample_buf, uint32_t num_samples, uint32_t num_dims, c_rng& rng);