#include <assert.h>
#include "triangle_mesh.h" 

c_triangle_mesh::c_triangle_mesh(const vertices_array& verts, 
	const normals_array& normals, 
	const tangents_array& tangents, 
	const uvs_array& uvs, 
	const face_indices_array& indices)
{
	assert(verts.size() > 0); 
	assert(indices.size() > 0);
	
	m_num_faces = indices.size(); 
	m_num_verts = verts.size(); 

	m_has_normal = (normals.size() > 0) ? true : false; 
	m_has_tangent = (tangents.size() > 0) ? true : false; 
	m_has_uv = (uvs.size() > 0) ? true : false; 
	
	m_verts.reserve(m_num_verts); 
	m_faces_indices.reserve(indices.size());
	std::copy(verts.begin(), verts.end(), back_inserter(m_verts)); 
	std::copy(indices.begin(), indices.end(), back_inserter(m_faces_indices));
	
	if (m_has_normal)
	{
		m_normals.reserve(m_num_verts); 
		std::copy(normals.begin(), normals.end(), back_inserter(m_normals)); 
	}
	
	if (m_has_tangent)
	{
		m_tangents.reserve(m_num_verts); 
		std::copy(tangents.begin(), tangents.end(), back_inserter(m_tangents));
	}
	
	if (m_has_uv)
	{
		m_uvs.reserve(m_num_verts);
		std::copy(uvs.begin(), uvs.end(), back_inserter(m_uvs)); 
	}

	// allocate the triangles 
	for (uint32_t i = 0; i < m_num_faces; ++i)
	{
		c_triangle_face triangle(this, i);
		m_triangles.push_back(triangle);
	}
}

//////////////////////////////////////////////////////////////////////////

void c_triangle_face::get_shading_geometry(const c_transform& o2w, 
	const c_differential_geometry& geom_dg, 
	PARAM_OUT c_differential_geometry *shading_dg) const 
{
	if (!m_mesh->has_normal() && !m_mesh->has_tangent())
	{
		*shading_dg = geom_dg;
		return;
	}

	// Compute barycentric coordinates for point
	float b[3]; 

	// Initialize _A_ and _C_ matrices for barycentrics
	float uv[3][2]; 
	get_uv(uv); 
	float A[2][2] =
	{ { uv[1][0] - uv[0][0], uv[2][0] - uv[0][0] },
	{ uv[1][1] - uv[0][1], uv[2][1] - uv[0][1] } };

	float C[2] = { geom_dg.u - uv[0][0], geom_dg.v - uv[0][1] };
	if (!solve_linear_system2x2(A, C, &b[1], &b[2])) {
		// Handle degenerate parametric mapping
		b[0] = b[1] = b[2] = 1.f/3.f;
	}
	else 
		b[0] = 1.f - b[1] - b[2];

	// Use _n_ and _s_ to compute shading tangents for triangle, _ss_ and _ts_
	vector3f ns; 
	vector3f ss, ts; 
	vector3f& n1 = m_mesh->m_normals[m_tri_face[0]]; 
	vector3f& n2 = m_mesh->m_normals[m_tri_face[1]]; 
	vector3f& n3 = m_mesh->m_normals[m_tri_face[2]]; 
	vector3f& s1 = m_mesh->m_tangents[m_tri_face[0]];
	vector3f& s2 = m_mesh->m_tangents[m_tri_face[1]];
	vector3f& s3 = m_mesh->m_tangents[m_tri_face[2]];

	if (m_mesh->has_normal())
	{
		vector3f v = (n1 * b[0] + n2 * b[1] + n3 * b[2]);
		ns = o2w.transform_vec3(v); 
		ns.normalize(); 
	}
	else 
		ns = geom_dg.nn; 

	if (m_mesh->has_tangent())
	{
		vector3f v = s1 * b[0] + s2 * b[1] + s3 * b[2];
		ss = o2w.transform_vec3(v); 
		ss.normalize(); 
	}
	else
		ss = normalize(geom_dg.dpdu); 

	ts = cross(ss, ns); 
	if (ts.length_squared() > 0.0f)
	{
		ts = normalize(ts); 
		ss = cross(ts, ns); 
	}
	else 
		build_coord_system(ns, &ss, &ts); 

	vector3f dndu, dndv; 
	// Compute $\dndu$ and $\dndv$ for triangle shading geometry
	if (m_mesh->has_normal())
	{
		// Compute deltas for triangle partial derivatives of normal
		float du1 = uv[0][0] - uv[2][0];
		float du2 = uv[1][0] - uv[2][0];
		float dv1 = uv[0][1] - uv[2][1];
		float dv2 = uv[1][1] - uv[2][1];
		float determinant = du1 * dv2 - dv1 * du2;
		if (determinant == 0.f)
			dndu = dndv = vector3f(0,0,0);
		else 
		{
			float inv_det = 1.f / determinant;
			dndu = ( dv2 * n1 - dv1 * n2) * inv_det;
			dndv = (-du2 * n1 + du1 * n2) * inv_det;
		}
	}
	else 
		dndu = dndv = vector3f(0,0,0);

	vector3f w_dndu = o2w.transform_vec3(dndu); 
	vector3f w_dndv = o2w.transform_vec3(dndv); 
	*shading_dg = c_differential_geometry(geom_dg.p, ss, ts, w_dndu, w_dndv, geom_dg.u, geom_dg.v, geom_dg.shape); 

	shading_dg->dudx = geom_dg.dudx;
	shading_dg->dvdx = geom_dg.dvdx; 
	shading_dg->dudy = geom_dg.dudy; 
	shading_dg->dvdy = geom_dg.dvdy; 
	shading_dg->dpdx = geom_dg.dpdx; 
	shading_dg->dpdy = geom_dg.dpdy;
}

void c_triangle_face::get_uv(float uv[3][2]) const 
{
	if (m_mesh->has_uvs()) 
	{
		uv[0][0] = m_mesh->m_uvs[m_tri_face[0]][0];
		uv[0][1] = m_mesh->m_uvs[m_tri_face[0]][1];
		uv[1][0] = m_mesh->m_uvs[m_tri_face[1]][0]; 
		uv[1][1] = m_mesh->m_uvs[m_tri_face[1]][1];
		uv[2][0] = m_mesh->m_uvs[m_tri_face[2]][0]; 
		uv[2][1] = m_mesh->m_uvs[m_tri_face[2]][1];
	}
	else 
	{
		uv[0][0] = 0.f;	uv[0][1] = 0.f; 
		uv[1][0] = 1.f; uv[1][1] = 0.f; 
		uv[2][0] = 1.f; uv[2][1] = 1.f; 
	}
}

bool c_triangle_face::intersects(const c_ray& ray, 
	PARAM_OUT float *t_hit, PARAM_OUT float *ray_epsilon, PARAM_OUT c_differential_geometry *geom_dg) const 
{
	point3f& p1 = m_mesh->m_verts[m_tri_face[0]];
	point3f& p2 = m_mesh->m_verts[m_tri_face[1]];
	point3f& p3 = m_mesh->m_verts[m_tri_face[2]];

	vector3f e1 = p2 - p1;
	vector3f e2 = p3 - p1; 

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

	vector3f dpdu, dpdv; 
	float uvs[3][2];
	get_uv(uvs);

	// Compute deltas for triangle partial derivatives (dpdu, dpdv)
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

	// Interpolate $(u,v)$ triangle parametric coordinates
	float b0 = 1 - b1 - b2; 
	float tu = b0 * uvs[0][0] + b1 * uvs[1][0] + b2 * uvs[2][0]; 
	float tv = b0 * uvs[0][1] + b1 * uvs[1][1] + b2 * uvs[2][1]; 

	// Test intersection against alpha texture, if present
	// @TODO


	// Fill in _DifferentialGeometry_ from triangle hit
	point3f hit_p = ray.evaluate_t(t);
	*geom_dg = c_differential_geometry(hit_p, dpdu, dpdv, vector3f(0,0,0), vector3f(0,0,0), tu, tv, this);

	*t_hit = t; 
	*ray_epsilon = 1e-3f * *t_hit; 

	return true; 
}