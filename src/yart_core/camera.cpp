#include "camera.h"
#include "film.h"

c_projective_camera::c_projective_camera(const c_transform& proj, const float screen_wnd[4], float lensr, float focal_d, const film_ptr& film)
	: super(film)
	, m_cam_to_screen(proj)
	, m_lens_radius(lensr)
	, m_focal_distance(focal_d)
{
	m_screen_to_raster = make_scale((float)(m_film->res_x()), 
									(float)(m_film->res_y()), 
									1.0f) * ; 
	
	
}
