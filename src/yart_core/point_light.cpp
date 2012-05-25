#include "point_light.h"


c_spectrum c_point_light::sample_l(const point3f& p, float p_eps, const c_light_sample& light_sample, 
	PARAM_OUT vector3f *wi, PARAM_OUT float *pdf, PARAM_OUT c_occlusion_tester *visibility) const
{
	vector3f dir = m_light_pos - p;

	// Compute incident direction
	*wi = normalize(dir); 
	
	*pdf = 1.0f;
	
	*visibility = c_occlusion_tester(p, p_eps, m_light_pos, 0.0f); 

	return m_intensity / dir.length_squared();
}

c_spectrum c_point_light::compute_flux() const 
{
	return m_intensity * 4 * M_PI; 
}
