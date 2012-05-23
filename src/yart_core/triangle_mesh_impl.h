#pragma once 

#include <assert.h>
#include "prerequisites.h"
#include "triangle_mesh.h"
#include "assimp_api_wrap.h"

class c_assimp_mesh_impl : public c_triangle_mesh_impl
{
public: 
	explicit c_assimp_mesh_impl(aiMesh *mesh);
	bool has_tengent() const; 
	bool has_normal() const;
	bool has_uv(uint32_t uv_set /* = 0 */) const; 
	uint32_t get_num_faces() const;
	uint32_t get_num_verts() const;
	triangle_face get_face(uint32_t idx) const;
	point3f get_vert(uint32_t idx) const;
	uv get_vert_uv(uint32_t vert_idx, uint32_t uv_set = 0) const;
	vector3f get_vert_tengent(uint32_t vert_idx) const; 
	vector3f get_vert_normal(uint32_t vert_idx) const; 
	void apply_transform(const c_transform& t); 

	aiMesh *m_ai_mesh; 
};

class c_assimp_face_impl : public c_triangle_face_impl
{
public:
	c_assimp_face_impl(assimp_mesh_impl_ptr& assimp_mesh, uint32_t face_idx);

private:
	assimp_mesh_impl_ptr m_mesh;
	uint32_t m_face_idx; 
}; 