#include "triangle_mesh_impl.h"

c_assimp_mesh_impl::c_assimp_mesh_impl(aiMesh *mesh) 
	: m_ai_mesh(mesh)
{}

bool c_assimp_mesh_impl::has_normal() const 
{
	assert(m_ai_mesh); 
	return m_ai_mesh->HasNormals(); 
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

triangle_face c_assimp_mesh_impl::get_face(uint32_t idx) const 
{
	assert(m_ai_mesh); 
	assert(idx < m_ai_mesh->mNumFaces); 
	aiFace *face = &m_ai_mesh->mFaces[idx];
	assert(face->mNumIndices == 3);

	return triangle_face(face->mIndices[0], face->mIndices[1], face->mIndices[2]); 
}

point3f c_assimp_mesh_impl::get_vert(uint32_t idx) const 
{
	assert(m_ai_mesh);
	assert(idx < m_ai_mesh->mNumVertices);

	return aivec3_to_cmlvec3(m_ai_mesh->mVertices[idx]); 
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

//////////////////////////////////////////////////////////////////////////

c_assimp_face_impl::c_assimp_face_impl(assimp_mesh_impl_ptr& assimp_mesh, uint32_t face_idx)
	: m_mesh(assimp_mesh)
	, m_face_idx(face_idx)
{
}