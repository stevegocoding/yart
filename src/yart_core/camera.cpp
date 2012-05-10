#include "camera.h"
#include "film.h"

#include "cml/matrix/inverse.h"

c_projective_camera::c_projective_camera(
	const c_transform& cam_to_world, 
	const c_transform& proj, 
	const float screen_wnd[4], 
	float lensr, 
	float focal_d, 
	film_ptr& film)
	: super(cam_to_world, film)
	, m_cam_to_screen(proj)
	, m_lens_radius(lensr)
	, m_focal_distance(focal_d)
{
	m_screen_to_raster = make_scale((float)(m_film->res_x()), 
									(float)(m_film->res_y()), 
									1.0f) * 
						 make_scale(1.0f / (screen_wnd[1]-screen_wnd[0]), 1.0f / (screen_wnd[2] - screen_wnd[3]), 1.0f) * 
						 make_translate(vector3f(-screen_wnd[0], -screen_wnd[3], 0)); 
	
	m_raster_to_screen = inverse_transform(m_screen_to_raster);
	m_raster_to_camera = inverse_transform(m_camera_to_world) * m_raster_to_screen;	
}

//////////////////////////////////////////////////////////////////////////

/*
c_perspective_camera::c_perspective_camera(
	const c_transform& cam_to_world, 
	const float screen_wnd[4], 
	float lensr, 
	float focal_d, 
	float fov, 
	film_ptr& film);
	*/