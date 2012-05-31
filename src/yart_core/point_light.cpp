#include "point_light.h"

c_point_light::c_point_light(const c_transform& l2w, const c_spectrum& intensity, uint32_t num_samples)
	: super(l2w, num_samples)
	, m_intensity(intensity)
{
	// Transform into the world space
	m_light_pos = l2w.transform_pt(point3f(0.0f, 0.0f, 0.0f));
}

c_spectrum c_point_light::sample_l(const point3f& p, float p_eps, const c_light_sample& light_sample, 
	PARAM_OUT vector3f *wi, PARAM_OUT float *pdf, PARAM_OUT c_occlusion_tester *visibility) const
{
	vector3f dir = m_light_pos - p;

	// Compute incident direction
	*wi = normalize(dir); 
	
	*pdf = 1.0f;
	
	*visibility = c_occlusion_tester(p, p_eps, m_light_pos, 0.0f); 

	float att = dir.length();

	return m_intensity;
}

c_spectrum c_point_light::compute_flux() const 
{
	return m_intensity * 4 * M_PI; 
}

float c_point_light::eval_pdf(const point3f& p, const vector3f& v) const 
{
	return 0; 
}


light_ptr make_point_light(const c_transform& light_to_world, const c_spectrum& intensity)
{
	light_ptr light = light_ptr(new c_point_light(light_to_world, intensity));
	return light; 
}