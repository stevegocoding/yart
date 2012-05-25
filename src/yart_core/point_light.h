#pragma once 

#include "prerequisites.h"
#include "light.h"

class c_point_light : public c_light
{
public:
	c_point_light(const c_transform& l2w, const c_spectrum& intensity); 
	
	virtual c_spectrum compute_le(const c_ray& ray) const; 

	virtual c_spectrum sample_l(const point3f& p, float p_eps, const c_light_sample& light_sample, 
		PARAM_OUT vector3f *wi, PARAM_OUT float *pdf, PARAM_OUT c_occlusion_tester *visibility) const;
 
	virtual bool is_delta() const { return true; }

	virtual c_spectrum compute_flux() const; 

private:

	point3f m_light_pos; 
	c_spectrum m_intensity;				// The intensity of the point light 
	
};