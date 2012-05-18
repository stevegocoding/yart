#pragma once 

#include <assert.h>
#include "prerequisites.h"
#include "triangle_mesh.h"
#include "assimp_api_wrap.h"

class c_assimp_mesh_impl : public c_triangle_mesh_impl
{
public: 
	explicit c_assimp_mesh_impl(aiMesh *mesh);
	bool has_normal() const;
	uint32_t get_num_faces() const;
	uint32_t get_num_verts() const;
	triangle_face get_face(uint32_t idx) const;
	point3f get_vert(uint32_t idx) const;
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