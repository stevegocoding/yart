#pragma once 

#include <assert.h>
#include "prerequisites.h"
#include "triangle_mesh.h"
#include "assimp_api_wrap.h"

class c_assimp_face_impl; 
typedef std::vector<c_assimp_face_impl> assimp_face_impl_array; 

class c_assimp_face_impl : public c_triangle_face_impl
{
public:
	c_assimp_face_impl(aiMesh *ai_mesh = NULL, uint32_t face_idx = 0);

	virtual bool intersects_impl(const c_ray& ray, 
		PARAM_OUT float *t_hit, 
		PARAM_OUT float *ray_epsilon, 
		PARAM_OUT c_differential_geometry *geom_dg) const; 
	
	virtual void get_shading_geometry_impl(const c_transform& o2w, 
		const c_differential_geometry& geom_dg, 
		PARAM_OUT c_differential_geometry *shading_dg) const;

	void get_uv_impl(float uv[3][2]) const; 

	aiMesh *m_ai_mesh;
	aiFace *m_ai_face;
	uint32_t m_face_idx; 
}; 

//////////////////////////////////////////////////////////////////////////

class c_assimp_mesh_impl : public c_triangle_mesh_impl
{
public: 
	explicit c_assimp_mesh_impl(aiMesh *mesh);
	~c_assimp_mesh_impl() {}

	bool has_tengent() const; 
	bool has_normal() const;
	bool has_uv(uint32_t uv_set /* = 0 */) const; 
	uint32_t get_num_faces() const;
	uint32_t get_num_verts() const;
	void apply_transform(const c_transform& t); 

	aiMesh* get_ai_mesh() const 
	{ 
		return m_ai_mesh; 
	}

	const c_triangle_face_impl *get_face_impl(uint32_t face_idx) const 
	{
		return &m_triangles[face_idx]; 
	}

private:
	void allocate_triangles(); 

	aiMesh *m_ai_mesh; 
	assimp_face_impl_array m_triangles;  
};
