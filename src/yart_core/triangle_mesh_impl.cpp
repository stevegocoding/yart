#include "triangle_mesh_impl.h"
#include "ray.h"
#include "geometry.h"

c_assimp_mesh_impl::c_assimp_mesh_impl(aiMesh *mesh) 
	: m_ai_mesh(mesh)
{
	allocate_triangles();
}

bool c_assimp_mesh_impl::has_tengent() const
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasTangentsAndBitangents();
}

bool c_assimp_mesh_impl::has_normal() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasNormals(); 
}

bool c_assimp_mesh_impl::has_uv(uint32_t uv_set) const
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasTextureCoords(uv_set); 
}

uint32_t c_assimp_mesh_impl::get_num_faces() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->mNumFaces; 
}

uint32_t c_assimp_mesh_impl::get_num_verts() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->mNumVertices;
}

void c_assimp_mesh_impl::apply_transform(const c_transform& t)
{
	assert(m_ai_mesh); 
	
	aiMatrix4x4 aim = cmlmat4x4_to_aimat4x4(t.get_matrix()); 
	
	for (uint32_t i = 0; i < m_ai_mesh->mNumVertices; ++i)
	{
		m_ai_mesh->mVertices[i] = aim * m_ai_mesh->mVertices[i]; 
	}
}

void c_assimp_mesh_impl::allocate_triangles()
{
	uint32_t num_faces = m_ai_mesh->mNumFaces; 
	m_triangles.reserve(num_faces);
	for (uint32_t i = 0; i < num_faces; ++i)
	{
		m_triangles.push_back(c_assimp_face_impl(m_ai_mesh, i)); 
	}
}

//////////////////////////////////////////////////////////////////////////

c_assimp_face_impl::c_assimp_face_impl(aiMesh *ai_mesh, uint32_t face_idx)
	: m_ai_mesh(ai_mesh)
	, m_face_idx(face_idx)
{
	m_ai_face = &(m_ai_mesh->mFaces[face_idx]);
}

bool c_assimp_face_impl::intersects_impl(const c_ray& ray, 
	PARAM_OUT float *t_hit, 
	PARAM_OUT float *ray_epsilon, 
	PARAM_OUT c_differential_geometry *geom_dg) const
{
	aiVector3D ray_d = aiVector3D(ray.d[0], ray.d[1], ray.d[2]); 
	aiVector3D ray_o = aiVector3D(ray.o[0], ray.o[1], ray.o[2]); 

	aiVector3D& p1 = m_ai_mesh->mVertices[m_ai_face->mIndices[0]];
	aiVector3D& p2 = m_ai_mesh->mVertices[m_ai_face->mIndices[1]];
	aiVector3D& p3 = m_ai_mesh->mVertices[m_ai_face->mIndices[2]]; 
	
    aiVector3D e1 = p2 - p1;
    aiVector3D e2 = p3 - p1; 
    
    // vector3f s1 = cross(ray.d, e2); 

    aiVector3D s1 = ray_d ^ e2;
	
	// float divisor = dot(s1, e1); 
    float divisor = s1 * e1; 
    if (divisor == 0)
        return false;
    
    float inv_div = 1.0f / divisor; 

    // Compute the first barycentric coordinates
    // vector3f d = ray.o - p1; 
	aiVector3D d = ray_o - p1; 

    // float b1 = dot(d, s1) * inv_div;
	float b1 = d * s1 * inv_div;
    if (b1 < 0.0f || b1 > 1.0f)
        return false;
	 
    // Compute the second barycentric coordinates 
    // vector3f s2 = cross(d, e1); 
	aiVector3D s2 = d ^ e1; 
    // float b2 = dot(ray.d, s2) * inv_div; 
	float b2 = ray_d * s2 * inv_div; 
    if (b2 < 0.0f || b1 + b2 > 1.0f)
        return false;

    // Compute t to intersection point 
    // float t = dot(e2, s2) * inv_div; 
	float t = e2 * s2 * inv_div; 
    if (t < ray.t_min || t > ray.t_max)
        return false; 
    
    // Compute the triangle partial derivatives 
	
    aiVector3D dpdu, dpdv; 
    float uvs[3][2];
    get_uv_impl(uvs);
    
    // Compute deltas for triangle partial derivatives (dpdu, dpdv)
    float du1 = uvs[0][0] - uvs[2][0]; 
    float du2 = uvs[1][0] - uvs[2][0]; 
    float dv1 = uvs[0][1] - uvs[2][1]; 
    float dv2 = uvs[1][1] - uvs[2][1]; 
    aiVector3D dp1 = p1 - p3; 
    aiVector3D dp2 = p2 - p3; 
    float determinant = du1 * dv2 - dv1 * du2; 
    if (determinant == 0.0f)
    {
        // Handle zero determinant for triangle partial derivative matrix
		aiVector3D e2_e1 = e2 ^ e1;
		e2_e1.Normalize(); 
		
		ai_build_coord_system(e2_e1, &dpdu, &dpdv); 
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
	*geom_dg = c_differential_geometry(hit_p, aivec3_to_cmlvec3(dpdu), aivec3_to_cmlvec3(dpdv), vector3f(0,0,0), vector3f(0,0,0), tu, tv, NULL);
	
	*t_hit = t; 
	*ray_epsilon = 1e-3f * *t_hit; 
	//*ray_epsilon = 0.0f; 

    return true; 
}

void c_assimp_face_impl::get_shading_geometry_impl(const c_transform& o2w, 
	const c_differential_geometry& geom_dg, 
	PARAM_OUT c_differential_geometry *shading_dg) const
{
	if (!m_ai_mesh->HasNormals() && !m_ai_mesh->HasTangentsAndBitangents())
	{
		*shading_dg = geom_dg;
		return;
	}

	// Compute barycentric coordinates for point
	float b[3]; 

	// Initialize _A_ and _C_ matrices for barycentrics
	float uv[3][2]; 
	get_uv_impl(uv); 
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
	aiVector3D ns; 
	aiVector3D ss, ts; 
	aiVector3D& n1 = m_ai_mesh->mNormals[m_ai_face->mIndices[0]]; 
	aiVector3D& n2 = m_ai_mesh->mNormals[m_ai_face->mIndices[1]]; 
	aiVector3D& n3 = m_ai_mesh->mNormals[m_ai_face->mIndices[2]]; 
	aiVector3D& s1 = m_ai_mesh->mTangents[m_ai_face->mIndices[0]];
	aiVector3D& s2 = m_ai_mesh->mTangents[m_ai_face->mIndices[1]];
	aiVector3D& s3 = m_ai_mesh->mTangents[m_ai_face->mIndices[2]];

	aiMatrix4x4 _o2w = cmlmat4x4_to_aimat4x4(o2w.get_matrix());

	if (m_ai_mesh->HasNormals())
	{
		aiVector3D v = (n1 * b[0] + n2 * b[1] + n3 * b[2]);
		ns = _o2w * v; 
		ns.Normalize(); 
	}
	else 
		ns = cmlvec3_to_aivec3(geom_dg.nn); 

	if (m_ai_mesh->HasTangentsAndBitangents())
	{
		aiVector3D v = s1 * b[0] + s2 * b[1] + s3 * b[2];
		ss = _o2w * v; 
		ss.Normalize(); 
	}
	else
		ss = cmlvec3_to_aivec3(normalize(geom_dg.dpdu)); 

	ts = ss ^ ns; 
	if (ts.SquareLength() > 0.0f)
	{
		ts.Normalize(); 
		ss = ts ^ ns; 
	}
	else 
		ai_build_coord_system(ns, &ss, &ts); 

	aiVector3D dndu, dndv; 
	// Compute $\dndu$ and $\dndv$ for triangle shading geometry
	if (m_ai_mesh->HasNormals())
	{
		// Compute deltas for triangle partial derivatives of normal
		float du1 = uv[0][0] - uv[2][0];
		float du2 = uv[1][0] - uv[2][0];
		float dv1 = uv[0][1] - uv[2][1];
		float dv2 = uv[1][1] - uv[2][1];
		float determinant = du1 * dv2 - dv1 * du2;
		if (determinant == 0.f)
			dndu = dndv = aiVector3D(0,0,0);
		else 
		{
			float inv_det = 1.f / determinant;
			dndu = ( dv2 * n1 - dv1 * n2) * inv_det;
			dndv = (-du2 * n1 + du1 * n2) * inv_det;
		}
	}
	else 
		dndu = dndv = aiVector3D(0,0,0);

	aiVector3D w_dndu = _o2w * dndu; 
	aiVector3D w_dndv = _o2w *dndv; 
	
	vector3f _w_dndu = aivec3_to_cmlvec3(w_dndu); 
	vector3f _w_dndv = aivec3_to_cmlvec3(w_dndv); 
	vector3f _ss = aivec3_to_cmlvec3(ss);
	vector3f _ts = aivec3_to_cmlvec3(ts); 
	*shading_dg = c_differential_geometry(geom_dg.p, _ss, _ts, _w_dndu, _w_dndv, geom_dg.u, geom_dg.v, geom_dg.shape); 

	shading_dg->dudx = geom_dg.dudx; 
	shading_dg->dvdx = geom_dg.dvdx; 
	shading_dg->dudy = geom_dg.dudy; 
	shading_dg->dvdy = geom_dg.dvdy; 
	shading_dg->dpdx = geom_dg.dpdx; 
	shading_dg->dpdy = geom_dg.dpdy;
	
}

void c_assimp_face_impl::get_uv_impl(float uv[3][2]) const 
{
	if (m_ai_mesh->HasTextureCoords(0))  
	{
		uv[0][0] = m_ai_mesh->mTextureCoords[0][m_ai_face->mIndices[0]].x;
		uv[0][1] = m_ai_mesh->mTextureCoords[0][m_ai_face->mIndices[0]].y;
		uv[1][0] = m_ai_mesh->mTextureCoords[0][m_ai_face->mIndices[1]].x;
		uv[1][1] = m_ai_mesh->mTextureCoords[0][m_ai_face->mIndices[1]].y;
		uv[2][0] = m_ai_mesh->mTextureCoords[0][m_ai_face->mIndices[2]].x;
		uv[2][1] = m_ai_mesh->mTextureCoords[0][m_ai_face->mIndices[2]].y;
	}
	else 
	{
		uv[0][0] = 0.f;	uv[0][1] = 0.f; 
		uv[1][0] = 1.f; uv[1][1] = 0.f; 
		uv[2][0] = 1.f; uv[2][1] = 1.f; 
	}
}