#include "reflection.h"


c_spectrum c_bxdf_base::sample_f(const vector3f& wo, vector3f& wi, float u1, float u2, float *pdf) const
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
	// wi = cosine_sample_hemisphere(u1, u2); 
}

c_spectrum c_bxdf_base::rho(const vector3f& wo, int num_samples, const samples_array_ptr& samples) const 
{
	return c_spectrum();
}

c_spectrum c_bxdf_base::rho(int num_samples, const samples_array_ptr& samples1, const samples_array_ptr& samples2) const
{
	return c_spectrum(); 

}

