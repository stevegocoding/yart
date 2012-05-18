#include <assert.h>
#include "triangle_mesh.h" 
#include "assimp_api_wrap.h"

void c_triangle_mesh::alloc_triangles_array()
{
	assert(m_impl);
	std::vector<triangle_face_ptr>().swap(m_triangles);
	m_triangles.reserve(m_impl->get_num_faces()); 
	
	for (uint32_t i = 0; i < m_impl->get_num_faces(); ++i)
	{
		m_triangles.push_back(triangle_face_ptr(new c_triangle_face(this, i)));
	}
}

//////////////////////////////////////////////////////////////////////////


bool c_triangle_face::intersects(const c_ray& ray, 
    PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT diff_geom_ptr& diff_geom) const 
{
    triangle_face face = m_mesh->get_face(m_face_idx);

    point3f p1 = m_mesh->get_vert(face[0]);
    point3f p2 = m_mesh->get_vert(face[1]);
    point3f p3 = m_mesh->get_vert(face[2]);

    vector3f e1 = p2 - p1;
    vector3f e2 = p3 - p2; 
    
    vector3f s1 = cross(ray.d, e2); 
    float divisor = dot(s1, e1); 
    
    if (divisor == 0)
        return false;
    
    float inv_div = 1.0f / divisor; 

    // Compute the first barycentric coordinates
    vector3f d = ray.o - p1; 
    float b1 = dot(d, s1) * inv_div;
    if (b1 < 0.0f || b1 > 1.0f)
        return false;

    // Compute the second barycentric coordinates 
    vector3f s2 = cross(d, e1); 
    float b2 = dot(ray.d, s2) * inv_div; 
    if (b2 < 0.0f || b1 + b2 > 1.0f)
        return false;

    // Compute t to intersection point 
    float t = dot(e2, s2) * inv_div; 
    if (t < ray.t_min || t > ray.t_max)
        return false; 
    
    // Compute the triangle partial derivatives 
	/* 
    vector3f dpdu, dpdv; 
    float uvs[3][2];
    get_uv(uvs);
    
    // Compute deltas for triangle partial derivatives 
    float du1 = uvs[0][0] - uvs[2][0]; 
    float du2 = uvs[1][0] - uvs[2][0]; 
    float dv1 = uvs[0][1] - uvs[2][1]; 
    float dv2 = uvs[1][1] - uvs[2][1]; 
    vector3f dp1 = p1 - p3; 
    vector3f dp2 = p2 - p3; 
    float determinant = du1 * dv2 - dv1 * du2; 
    if (determinant == 0.0f)
    {
        // Handle zero determinant for triangle partial derivative matrix
		build_coord_system(normalize(cross(e2, e1)), &dpdu, &dpdv); 
    }
	else 
	{
		float inv_det = 1.0f / determinant; 
		dpdu = (dv2 * dp1 - dv1 * dp2) * inv_det;
		dpdv = (-du2 * dp1 + du1 * dp2) * inv_det;
	}
	*/
	
	*t_hit = t; 
	*ray_epsilon = 1e-3f * *t_hit; 

    return true; 
}

