#include <assert.h>
#include "triangle_mesh.h" 
#include "assimp_api_wrap.h"

void c_triangle_mesh::allocate_triangles()
{
	uint32_t num_faces = m_impl->get_num_faces(); 
	m_triangles.reserve(num_faces); 

	for (uint32_t i = 0; i < num_faces; ++i)
	{
		triangle_face_ptr face = triangle_face_ptr(new c_triangle_face(m_impl->get_face_impl(i))); 
		m_triangles.push_back(face); 
	}
}

//////////////////////////////////////////////////////////////////////////

bool c_triangle_face::intersects(const c_ray& ray, 
    PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT c_differential_geometry *geom_dg) const 
{
	 bool is_hit = m_face_impl->intersects_impl(ray, t_hit, ray_epsilon, geom_dg); 
	 geom_dg->shape = this; 

	 return is_hit; 
}

void c_triangle_face::get_shading_geometry(const c_transform& o2w, 
	const c_differential_geometry& geom_dg, 
	PARAM_OUT c_differential_geometry *shading_dg) const 
{
	m_face_impl->get_shading_geometry_impl(o2w, geom_dg, shading_dg); 
}
