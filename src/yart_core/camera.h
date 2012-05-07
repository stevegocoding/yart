#pragma once

#include "prerequisites.h"
#include "transform.h"

class c_camera
{
public:
	explicit c_camera(const film_ptr& film)
		: m_film(film)
	{
	}
	
	virtual ~c_camera() {} 

	virtual float generate_ray(const c_camera_sample& cam_sample, c_ray& ray) = 0;

protected:
	film_ptr m_film; 
};

class c_projective_camera : public c_camera
{
	typedef c_camera super;
public:
	c_projective_camera(const c_transform& proj, const float screen_wnd[4], float lensr, float focal_d, const film_ptr& film); 

protected:
	c_transform m_cam_to_screen;
	c_transform m_screen_to_raster;
	float m_lens_radius;
	float m_focal_distance; 
};